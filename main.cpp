#include <fstream>
#include <iostream>

#include "spider.h"

// #define ONLY_HUE_BR

#define STATE_FILE_NAME "save_state.sav"
#define LOG_FILE "log_file.txt"

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
      // The [=] you're referring to is part of the capture list for the
      // lambda expression. This tells C++ that the code inside the lambda
      // expression is initialized so that the lambda gets a copy of all the
      // local variables it uses when it's created. This is necessary for
      // the lambda expression to be able to refer to  factor and offset,
      // which are local variables inside the function.

      // If you replace the [=] with [], you'll get a compiler error because
      // the code inside the lambda expression won't know what the variables
      // offset and factor refer to. Many compilers give good diagnostic
      // error messages if you do this, so try it and see what happens!
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

int main() {
	std::string url("http://homepages.dcc.ufmg.br/~berthier/");
  {
    ThreadPool pool(36);

		auto outbound_links = pool.enqueue([&url] {
			Spider spider(url);
			while(spider.crawl()){
				spider.printStatus();
			};
			return spider.getOutboundLinks();
		});

		for(auto &x : outbound_links.get()) {
			pool.enqueue([&x] {
				Spider spider(x);
				while(spider.crawl()){
					spider.printStatus();
				};
				return 0;
			});
		}
			// std::cout << x << '\n';

    // auto f1 = pool.enqueue([] { return 1; });
		//
    // auto f2 = pool.enqueue([] { return 2; });
		//
    // std::cout << (f1.get() + f2.get()) << std::endl;
  }

  return 0;
}
