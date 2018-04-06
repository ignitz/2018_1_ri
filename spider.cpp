#include "spider.h"

bool orderBySize(std::string x, std::string y) {
  return (x.length() < y.length());
};

Spider::Spider(std::string url) {
  this->domain = this->spider.getUrlDomain(url.c_str());
  if (url.substr(0, 8).compare("https://") != 0 &&
      url.substr(0, 7).compare("http://") != 0) {
    std::cerr << "Not a valid URL: " << url << '\n';
    throw;
  }

  // this->spider.put_VerboseLogging(true);

  this->url = url.c_str();
  this->base_domain = this->spider.getBaseDomain(this->domain);
  this->spider.Initialize(this->domain); // spider.Initialize("www.uol.com.br");
  this->spider.AddUnspidered(
      this->url); // spider.AddUnspidered("https://www.uol.com.br/");
  this->spider.put_MaxResponseSize(MAX_RESPONSE_SIZE);
#ifdef DEBUG
  std::cout << "Spider constructed" << '\n';
#endif
}

Spider::~Spider() {
  this->ClearOutboundLinks();
#ifdef DEBUG
  std::cout << "Spider deleted" << '\n';
#endif
}

void Spider::crawl() {
#ifdef DEBUG
  std::cout << "Spider crawl in " << domain.getString() << '\n';
#endif
  this->spider.CrawlNext();
  this->html = this->spider.lastHtml();
  // std::cout << this->html.getString() << '\n';
#ifdef DEBUG
  std::cout << "Spider done in " << domain.getString() << '\n';
#endif
}

std::vector<std::string> Spider::getOutboundLinks() {
  std::vector<std::string> result;
  std::string url;

  int size = this->spider.get_NumOutboundLinks();
  for (int i = 0; i < size; i++) {
    url = spider.getOutboundLink(i);
    result.push_back(url);
  }

  std::sort(result.begin(), result.end(), orderBySize);

  return result;
};

void Spider::ClearOutboundLinks() {
#ifdef DEBUG
  std::cout << "Clear Outbound Links" << '\n';
#endif
  this->spider.ClearOutboundLinks();
};

std::vector<std::string> Spider::getUnspideredUrls() {
  std::vector<std::string> result;
  std::string url;
  int size = this->spider.get_NumUnspidered();
  for (int i = 0; i < size; i++) {
    url = this->spider.getUnspideredUrl(i);
    result.push_back(url);
  }
  std::sort(result.begin(), result.end(), orderBySize);

  return result;
};

std::string Spider::getUrl() {
  return std::string(this->url.getString());
};

std::string Spider::getHtml() {
  return std::string(this->spider.lastHtml());
}

void Spider::playground(){
  // int size = this->spider.get_NumOutboundLinks();
  // for (int i = 0; i < size; i++) {
  //   std::cout << spider.getOutboundLink(i) << '\n';
  // }
  //
  // size = this->spider.get_NumUnspidered();
  // for (size_t i = 0; i < size; i++) {
  //   std::cout << spider.getUnspideredUrl(i) << '\n';
  // }


  // std::cout << this->spider.lastErrorHtml() << '\n';
  // std::cout << "|||" << this->spider.lastUrl() << "||" << this->spider.lastHtml() << '\n';
};
