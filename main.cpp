#include <fstream>
#include <iostream>

#include "spider.h"

#define ONLY_HUE_BR

// #define STATE_FILE_NAME "save_state.sav"
#define UNIQUEIDs_FILE "unique_id_file.txt"
#define OUTPUT_FILE "output_file.txt"

// Initial URL tp crawl
#define INITIAL_URL "http://uol.com.br/"

#define MAX_THREADS 1000

#include <unistd.h>
#include <string>

#include <deque>
#include <functional>
#include <future>
#include <mutex>

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

#include <future>

#include <utility>

/*****************************************/
class OutputFile {
 private:
  std::fstream obj_file;

 public:
  OutputFile(std::string file_name) {
    if (!this->obj_file.is_open())
      this->obj_file.open(file_name, std::fstream::app);
    // this->obj_file.open(file_name);
  };
  virtual ~OutputFile() { this->obj_file.close(); };

  void write(std::string url, std::string html) {
    // find '/' and remove
    this->obj_file << "|||" << url << '|';
    if (html.length() > 0) {
      html.erase(std::remove(html.begin(), html.end(), '|'), html.end());
      this->obj_file << html;
    }
    // this->obj_file << url << '\n';
    // html.erase(std::remove(html.begin(), html.end(), '|'), html.end());
    // this->obj_file << html;
  };

  void write(std::string s) { this->obj_file << s; };
};

/*****************************************/
class CollectionID {
 private:
  std::fstream obj_file;
  std::vector<unsigned long long> vec_ids;

 public:
  CollectionID() {
    this->obj_file.open(UNIQUEIDs_FILE);

    if (!this->obj_file.is_open()) {
      this->obj_file.clear();
      this->obj_file.open(UNIQUEIDs_FILE, std::ios::out);
      this->obj_file.close();
      this->obj_file.open(UNIQUEIDs_FILE, std::ios::in);
    }
    else {
      this->obj_file.close();
      this->obj_file.open(UNIQUEIDs_FILE, std::ios::in);
    }

    std::string getcontent;
    while (true) {
      obj_file >> getcontent;
      if (obj_file.eof()) break;
      if (getcontent.length() > 0) vec_ids.push_back(std::stoull(getcontent));
    };

    for (auto x : vec_ids) {
      std::cout << FAIL << x << ENDC << '\n';
    }

    this->obj_file.close();
  };
  virtual ~CollectionID() { this->obj_file.close(); };

  void write(std::string str) {
    this->obj_file.open(UNIQUEIDs_FILE, std::fstream::app);
    this->obj_file << str << '\n';
    this->obj_file.close();
  }
  void write(unsigned long long id) { write(std::to_string(id)); }

  void add_id(unsigned long long id) {
    this->write(id);
    vec_ids.push_back(id);
  };

  bool find_id(unsigned long long id) {
    for (const auto &x : vec_ids)
      if (x == id) return true;
    return false;
  };

  size_t get_many_ids() {
    return vec_ids.size();
  };
};

/*****************************************/
class ThreadPool {
 public:
  using Task = std::function<void()>;

  explicit ThreadPool(std::size_t numThreads) { start(numThreads); };
  ~ThreadPool() { stop(); };

  template <class T>
  auto enqueue(T task) -> std::future<decltype(task())> {
    auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(
        std::move(task));
    {
      std::unique_lock<std::mutex> lock{mEventMutex};
      mTasks.emplace([=] { (*wrapper)(); });
    }

    mEventVar.notify_one();
    return wrapper->get_future();
  }

 private:
  std::vector<std::thread> mThreads;

  std::condition_variable mEventVar;

  std::mutex mEventMutex;
  bool mStopping = false;

  std::queue<Task> mTasks;

  void start(std::size_t numThreads) {
    for (std::size_t i = 0; i < numThreads; ++i) {
      mThreads.emplace_back([=] {
        while (true) {
          Task task;
          {
            std::unique_lock<std::mutex> lock{mEventMutex};

            mEventVar.wait(lock, [=] { return mStopping || !mTasks.empty(); });

            if (mStopping && mTasks.empty()) break;

            task = std::move(mTasks.front());
            mTasks.pop();
          }
          task();
        }
      });
    }
  };
  void stop() noexcept {
    {
      std::unique_lock<std::mutex> lock{mEventMutex};
      mStopping = true;
    }

    mEventVar.notify_all();

    for (auto &thread : mThreads) thread.join();
  }
};

/*****************************************/
class Main {
 private:
  OutputFile *output;
  CollectionID *col_id;
  std::vector<Spider *> mSpiders;
  std::vector<std::string> outbound_links;
  std::vector<
      std::future<std::tuple<bool, std::vector<std::string>, std::string,
                             std::string, unsigned long long>>>
      mFutures;

  std::vector<int> indexes_to_delete;

  bool isEnough;

 public:
  Main(std::string initial_url, std::string output_file_name) {
    this->output = new OutputFile(output_file_name);
    this->col_id = new CollectionID();

    // this->mSpiders.push_back(new Spider(initial_url));
    std::fstream obj_file;
    obj_file.open("onde_parou.txt");

    // trecho temporário, apagar
    if (!obj_file.is_open()) {
      obj_file.clear();
      obj_file.open("onde_parou.txt", std::ios::out);
      obj_file.close();
      obj_file.open("onde_parou.txt", std::ios::in);
    }
    else {
      obj_file.close();
      obj_file.open("onde_parou.txt", std::ios::in);
    }

    std::string getcontent;
    while (true) {
      obj_file >> getcontent;
      if (obj_file.eof()) break;
      if (getcontent.length() > 0) this->mSpiders.push_back(new Spider(getcontent));
    };

    for (auto x : this->mSpiders) {
      std::cout << FAIL << x->getUrl() << ENDC << '\n';
    }

    obj_file.close();
    ////////////////////////////////

  };
  ~Main() {
    this->output->write("|||");
    std::cout << "-----------------------------------------------------------------------\n";
    delete this->output;
  };

  void manage_crawl() {
    isEnough = false;
    std::future<std::tuple<bool, std::vector<std::string>, std::string,
                           std::string, unsigned long long>>
        result;

    int count = 1;

    Spider *each_spider;

    while (!isEnough && !mSpiders.empty()) {
      ThreadPool pool(MAX_THREADS);
      size_t numSpiders = mSpiders.size();
      for (size_t i = 0; i < numSpiders && i < MAX_THREADS; ++i) {
        each_spider = mSpiders[i];
        result = pool.enqueue([=] {
          bool check_crawl = each_spider->crawl();
          each_spider->printStatus();
          // each_spider->printLinks();
          auto vec_of_out_links = each_spider->getOutboundLinks();
          auto url_content = each_spider->getUrl();
          auto html_content = each_spider->getHtml();
          auto unique_id = each_spider->getUniqueId();
          return std::tuple<bool, std::vector<std::string>, std::string,
                            std::string, unsigned long long>(
              std::move(check_crawl), std::move(vec_of_out_links),
              std::move(url_content), std::move(html_content),
              std::move(unique_id));
        });

        mFutures.push_back(std::move(result));
      }

      #ifdef DEBUG
      std::cout << WARNING << "Done dispatch threads " << ENDC << '\n';
      #endif

      // for avoid DDoS check
      std::this_thread::sleep_for(std::chrono::seconds(2));

      size_t numFutures = mFutures.size();
      for (size_t i = 0; i < numFutures; i++) mFutures[i].wait();

      for (size_t i = 0; i < numFutures; i++) {
        #ifdef DEBUG
        std::cout << WARNING << "Try to get future in " << i << ENDC << '\n';
        #endif
        auto aux_tuple = mFutures[i].get();
        #ifdef DEBUG
        std::cout << WARNING << "Catch future in " << i << ENDC << '\n';
        #endif
        if (!std::get<0>(aux_tuple)) indexes_to_delete.push_back(i);

        auto aux = std::get<1>(aux_tuple);
        if (aux.size() > 0)
          outbound_links.insert(outbound_links.end(), aux.begin(), aux.end());
        unsigned long long id = std::get<4>(aux_tuple);
        if (col_id->find_id(id)) {
          #ifdef DEBUG
          std::cout << WARNING << "continue" << ENDC << '\n';
          #endif
          continue;
        } else {
          #ifdef DEBUG
          std::cout << WARNING << "add_id " << id << ENDC << '\n';
          #endif
          col_id->add_id(id);
        }
        #ifdef DEBUG
        std::cout << WARNING << "Writing " << std::get<2>(aux_tuple) << " to file!\n" << ENDC;
        #endif
        output->write(std::get<2>(aux_tuple), std::get<3>(aux_tuple));
      }

      if (indexes_to_delete.size() > 0) {
        for (auto i = indexes_to_delete.size(); i > 0; i--) {
          std::cout << GREEN << "delete spider index " << (i - 1) << ENDC
                    << '\n';
          delete mSpiders[indexes_to_delete[i - 1]];
          mSpiders.erase(mSpiders.begin() + indexes_to_delete[i - 1]);
        }
      }

      // TODO: check if already unique

      // remove duplicate urls
      // std::sort(outbound_links.begin(), outbound_links.end());
      // outbound_links.erase(
      //     std::unique(outbound_links.begin(), outbound_links.end()),
      //     outbound_links.end());

      // for (auto x : outbound_links)
      //   std::cout << x << '\n';

      CkUrl util_url;
      for (const auto &new_url : outbound_links) {
        if (countDepth(new_url) > MAX_DEPTH) continue;
        #ifdef ONLY_HUE_BR
        if (check_if_not_BR(new_url)) continue;
        #endif
        util_url.ParseUrl(new_url.c_str());
        if (mSpiders.empty()) {
          try {
            Spider *aux_new_spider = new Spider(new_url);
            mSpiders.push_back(aux_new_spider);
          } catch (...) {
            std::cout << FAIL << "Exception in constructor of spider:\n"
                      << "\tURL = " << new_url << ENDC << '\n';
          }
        } else {
          bool url_not_exist = true;
          auto size = mSpiders.size();
          for (size_t i = 0; i < size; i++) {
            if (mSpiders[i]->getBaseDomain().compare(
                    getBaseDomain(util_url.host())) == 0) {
              mSpiders[i]->AddUnspidered(new_url);
              url_not_exist = false;
              break;
            }
          }
          if (url_not_exist) {
            try {
              Spider *aux_new_spider = new Spider(new_url);
              mSpiders.push_back(aux_new_spider);
            } catch (...) {
              std::cout << FAIL << "Exception in constructor of spider:\n"
                        << "\tURL = " << new_url << ENDC << '\n';
            }
          }
        }
      }

      #ifdef DEBUG
      std::cout << GREEN << "Done a loop" << ENDC << '\n';
      #endif
      // print all url mSpiders
      #ifdef DEBUG
      std::cout << WARNING << "Save state" << ENDC << '\n';
      #endif
      std::fstream save_urls_file("onde_parou.txt", std::ios::out);
      std::string strTemp;
      for (auto x : mSpiders) {
        std::cout << BOLD << x->getUrl() << ENDC << '\n';
        strTemp = x->getUrl();
        if (strTemp.find(' ') == std::string::npos) {
          save_urls_file << strTemp << '\n';
        }
        else {
          while (strTemp.find(' ') != std::string::npos) {
            strTemp.replace(strTemp.begin() + strTemp.find(' '), strTemp.begin() + strTemp.find(' ') + 1, "%20");
          }
        }
      }
      save_urls_file.close();

      indexes_to_delete.clear();
      outbound_links.clear();
      mFutures.clear();
      // if (count == 1) {
      //   isEnough = true;
      // }
      // Para quando tiver um milhão de páginas
      if (col_id->get_many_ids() >= 1000000) {
        isEnough = true;
      }
      count++;
    }
  };
};

/*****************************************/

void test1() {
  Main* mainoso = new Main(INITIAL_URL, OUTPUT_FILE);
  mainoso->manage_crawl();
  delete mainoso;
}

int main() {
  test1();
  return 0;
}
