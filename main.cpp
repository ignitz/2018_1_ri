#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "spider.h"

// #define ONLY_HUE_BR

#define STATE_FILE_NAME "save_state.sav"
#define LOG_FILE "log_file.txt"

// struct name_t {
// 	/* data */
// };
// #include <functional>
#include <string>
#include <unistd.h>

#include <mutex>
#include <deque>
#include <future>
#include <functional>


std::mutex mu;
std::deque<std::packaged_task<int()> > task_q;

using namespace std;

int factorial(int N) {
	int res = 1;
	for (int i=N; i>1; i--)
		res *= i;

	return res;
};

void thread_1() {
	for (int i=0; i<10000; i++) {
		std::packaged_task<int()> t;
		{
			std::lock_guard<std::mutex> locker(mu);
			if (task_q.empty())
				continue;
			t = std::move(task_q.front());
			task_q.pop_front();
		}
		t();
	}
};

void test_thread() {
  // const std::string initial_url = "http://homepages.dcc.ufmg.br/~berthier/";
  // Spider s1(initial_url);
  // std::vector<std::thread> vec_thr;

	std::thread th(thread_1);

	std::packaged_task<int()> t(bind(factorial, 6));
	std::future<int> ret = t.get_future();
	std::packaged_task<int()> t2(bind(factorial, 9));
	std::future<int> ret2 = t2.get_future();
	{
		std::lock_guard<std::mutex> locker(mu);
		task_q.push_back(std::move(t));
		task_q.push_back(std::move(t2));
	}
	cout << "I see: " << ret.get() << endl;
	cout << "I see: " << ret2.get() << endl;

	th.join();

  // pass the constructor parameters you would have passed to std::thread
  // to the emplace_back() function - they are forwarded to the thread that
  // is constructed "in place" inside the vector
  // vec_thr.emplace_back(&Spider::init, &s1);
	// // vec_thr.push_back(std::move(t1), str);
	//
  // // Don't forget to join all your threads to make sure
  // // they complete before moving on/exiting
  // for (auto &t : vec_thr) {
	// 		t.join();
	// }
};

void manage() {
  // Spider spider("http://homepages.dcc.ufmg.br/~berthier/");
  // Spider
  // spider("http://homepages.dcc.ufmg.br/~berthier/short_biography.htm");
  Spider spider("https://www.uol.com.br/");
  // Spider spider("https://gizmodo.uol.com.br/");
  // Spider spider("http://gizmodo.uol.com.br");

  while (spider.crawl()) {
    spider.removeUnspideredByDepth(MAX_DEPTH);
    spider.printStatus();
    spider.printLinks();
    sleep(1);
  };
};

int main() {
  test_thread();
  // manage();
  return 0;
}
