#ifndef SPIDER_H
#define SPIDER_H
#include <CkSpider.h>
#include <CkString.h>
#include <CkUrl.h>

#include <iostream>

#include <queue>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#include <algorithm> // sort
#include <functional> // hash
#include <unistd.h> // sleep

#include "utils.h"

#define DEBUG

// Max of 2MB
#define MAX_RESPONSE_SIZE 2097152
// Evitar URLs com mais de 7 palavras.
#define MAX_DEPTH 7

class Spider {
private:
  CkSpider spider;
  CkString html;
  CkString url;
  CkString domain;
  CkString base_domain;
  int depth;

  std::queue<std::string> queue;

public:
  Spider(std::string);
  virtual ~Spider();

  void setUrl(std::string);

  bool crawl();
  std::vector<std::string> getSpideredUrls();
  std::vector<std::string> getUnspideredUrls();
  std::vector<std::string> getOutboundLinks();

  void updateInfo();
  void removeUnspideredByDepth(int);
  void AddUnspidered(std::string);

  void printStatus();
  void printLinks();

  std::string getUrl();
  std::string getHost();
  std::string getBaseDomain();
  std::string getHtml();

  unsigned long long getUniqueId();
};
#endif
