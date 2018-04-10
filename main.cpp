#include <fstream>
#include <iostream>

#include "spider.h"

#define ONLY_HUE_BR

// #define STATE_FILE_NAME "save_state.sav"
#define UNIQUEIDs_FILE "unique_id_file.txt"
#define OUTPUT_FILE "output_file.txt"
#define MAX_THREADS 250

#include <string>
#include <unistd.h>

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

class OutputFile {
private:
  std::fstream obj_file;

public:
  OutputFile (std::string file_name){
    if (!this->obj_file.is_open())
      // this->obj_file.open(file_name, std::fstream::app);
      this->obj_file.open(file_name);
  };
  virtual ~OutputFile (){
    this->obj_file.close();
  };

  void write(std::string url, std::string html) {
    // find '/' and remove
    this->obj_file << "|||" << url << '|';
    html.erase(std::remove(html.begin(), html.end(), '|'), html.end());
    this->obj_file << html;
    // this->obj_file << url << '\n';
    // html.erase(std::remove(html.begin(), html.end(), '|'), html.end());
    // this->obj_file << html;
  };

  void write(std::string s) {
    this->obj_file << s;
  };
};

class CollectionID {
private:
  std::fstream obj_file;
public:
  CollectionID (){
    if (!this->obj_file.is_open()) {
      this->obj_file.open(UNIQUEIDs_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
      std::cout << "open" << '\n';
    }

    this->obj_file.seekg(0, this->obj_file.beg);
    // this->obj_file.seekp(0, this->obj_file.beg);

    std::string getcontent;

    while(! obj_file.eof())
    {
      obj_file >> getcontent;
      std::cout << getcontent << '\n';
    }

    // std::string line;
    // std::getline(this->obj_file, line);
    // while (std::getline(this->obj_file, line)) {
    //   std::cout << line << '\n';
    // }
    // std::cout << line << '\n';
  };
  virtual ~CollectionID (){
    this->obj_file.close();
  };

  void write(std::string str) {
    this->obj_file << str;
  }
};

/*****************************************/
class ThreadPool {
public:
  using Task = std::function<void()>;

  explicit ThreadPool(std::size_t numThreads) { start(numThreads); };
  ~ThreadPool() { stop(); };

  template <class T> auto enqueue(T task) -> std::future<decltype(task())> {
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

            if (mStopping && mTasks.empty())
              break;

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

    for (auto &thread : mThreads)
      thread.join();
  }
};
/*****************************************/

/*****************************************/
#define INITIAL_URL "http://uol.com.br/"
// #define INITIAL_URL "http://homepages.dcc.ufmg.br/~berthier/"
// #define INITIAL_URL \
  "http://www.asist.org/about/awards/best-information-science-book-award/"     \
  "best-information-science-book-award-past-winners"

int main1() {
  OutputFile output(OUTPUT_FILE);
  std::vector<Spider *> mSpiders;
  mSpiders.push_back(new Spider(INITIAL_URL));

  std::vector<std::string> outbound_links;
  std::vector<std::future<std::tuple<bool, std::vector<std::string>,
                                     std::string, std::string, unsigned long long>>>
      mFutures;

  std::vector<int> indexes_to_delete;

  // to lambda function
  Spider *each_spider;

  {
    bool isEnough = false;
    std::future<std::tuple<bool, std::vector<std::string>, std::string, std::string,
                           unsigned long long>>
        result;

    int count = 0;

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
          return std::tuple<bool, std::vector<std::string>, std::string, std::string,
                            unsigned long long>(
              std::move(check_crawl), std::move(vec_of_out_links), std::move(url_content),
              std::move(html_content), std::move(unique_id));
        });

        mFutures.push_back(std::move(result));
      }

      size_t numFutures = mFutures.size();
      for (size_t i = 0; i < numFutures; i++)
        mFutures[i].wait();

      for (size_t i = 0; i < numFutures; i++) {
#ifdef DEBUG
        std::cout << WARNING << "Try to get future in " << i << ENDC << '\n';
#endif
        auto aux_tuple = mFutures[i].get();
#ifdef DEBUG
        std::cout << WARNING << "Catch future in " << i << ENDC << '\n';
#endif
        if (!std::get<0>(aux_tuple))
          indexes_to_delete.push_back(i);

        auto aux = std::get<1>(aux_tuple);
        if (aux.size() > 0)
          outbound_links.insert(outbound_links.end(), aux.begin(), aux.end());

        output.write(std::get<2>(aux_tuple), std::get<3>(aux_tuple));

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
        if (countDepth(new_url) > MAX_DEPTH)
          continue;
#ifdef ONLY_HUE_BR
        if (check_if_not_BR(new_url))
          continue;
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
      // print all url mSpiders
      for (auto x : mSpiders) {
        std::cout << BOLD << x->getUrl() << ENDC << '\n';
      }
#endif

      indexes_to_delete.clear();
      outbound_links.clear();
      mFutures.clear();
      if (count == 14) {
        isEnough = true;
      }
      count++;
      // for avoid DDoS check
      // std::this_thread::sleep_for(std::chrono::seconds(5));
    }
  }
  output.write("|||");
  return 0;
}

int main() {
  // main1();
  CollectionID test;
  return 0;
}
