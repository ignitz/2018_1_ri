#include <iostream>
#include <thread>
#include <fstream>
#include <vector>

#include "spider.h"

#define DEBUG

#define STATE_FILE_NAME "save_state.sav"
#define LOG_FILE "log_file.txt"

// struct name_t {
// 	/* data */
// };

int main() {
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

	Spider spider("https://www.uol.com.br/");
	spider.crawl();
	// std::cout << spider.getHtml() << '\n';
	auto test = spider.getUnspideredUrls();
	for (auto x : test)
		std::cout << x << '\n';

	return 0;
}
