#ifndef SPIDER_H
#define SPIDER_H
#include <CkSpider.h>
#include <CkString.h>

#include <iostream>

#include <queue>
#include <string>
#include <vector>

#include <algorithm> // sort

#define DEBUG

#define MAX_RESPONSE_SIZE 4096
//  Max of 4MB

class Spider {
private:
  CkSpider spider;
  CkString html;
  CkString url;
  CkString domain;
  CkString base_domain;
  std::queue<std::string> queue;

public:
  Spider(std::string);
  virtual ~Spider();

  void crawl();
  std::vector<std::string> getOutboundLinks();
  void ClearOutboundLinks();

  std::vector<std::string> getUnspideredUrls();

  std::string getUrl();
  std::string getHtml();

  void playground();
};
#endif
