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
#include <functional>
#include <string>
#include <unistd.h>


int test1() {
  // try to open the LOG_FILE
  // std::ofstream log_file;
  // log_file.open(LOG_FILE);
  //
  // if (!log_file.is_open()) {
  // 	std::cout << "Cannot open log file" << '\n';
  // 	return -1;
  // }
  //
  // log_file.close();

  // Spider spider("https://www.uol.com.br/");
  Spider spider("https://www.wikipedia.org/");

  spider.crawl();
  spider.printStatus();
  spider.printLinks();
  auto outbounds = spider.getOutboundLinks();

  std::cout << FAIL << outbounds[0] << ENDC << '\n' << '\n';

  Spider spiderman(outbounds[0]);
  spiderman.crawl();
  spiderman.printStatus();
  spiderman.printLinks();

  // std::cout << countDepth("https://creativecommons.org/licenses/by-sa/3.0/")
  // << '\n';
  return 0;
};

void test2() {
};

void test3() {
	Spider spider("http://homepages.dcc.ufmg.br/~berthier/");
	// Spider spider("http://homepages.dcc.ufmg.br/~berthier/short_biography.htm");
	// Spider spider("https://www.uol.com.br/");
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
  test3();
  return 0;
}
