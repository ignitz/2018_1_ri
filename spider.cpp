#include "spider.h"

// Order by length of String
bool orderBySize(std::string x, std::string y) {
  return (x.length() < y.length());
};

// Order by depth of URL
bool orderByDepth(std::string url1, std::string url2) {
  return (countDepth(url1) < countDepth(url2));
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
  this->depth = countDepth(url);
#ifdef DEBUG
  std::cout << WARNING << "Spider constructed" << ENDC << '\n';
#endif
};

Spider::~Spider() {
  this->spider.ClearFailedUrls();
  this->spider.ClearOutboundLinks();
  this->spider.ClearSpideredUrls();
#ifdef DEBUG
  std::cout << WARNING << "Spider deleted" << ENDC << '\n';
#endif
};

void Spider::setUrl(std::string url) { this->url.setString(url.c_str()); };

bool Spider::crawl() {
#ifdef DEBUG
  std::cout << WARNING << "Spider crawl in " << domain.getString() << ENDC
            << '\n';
#endif
  bool result = this->spider.CrawlNext();
  this->html = this->spider.lastHtml();
  this->updateInfo();
#ifdef DEBUG
  std::cout << WARNING << "Spider done in " << domain.getString() << ENDC
            << '\n';
#endif
  return result;
};

void Spider::updateInfo() {
#ifdef DEBUG
  std::cout << WARNING << "updateInfo" << ENDC << '\n';
#endif
  this->url.setString(this->spider.lastUrl());
  this->domain = this->spider.getUrlDomain(this->url.getString());
  this->base_domain = this->spider.getBaseDomain(this->domain);
  this->depth = countDepth(url.getString());
};

void Spider::removeUnspideredByDepth(int min_depth) {
  int size = this->spider.get_NumUnspidered();

  int i = 0;
  while (i < size) {
    if (countDepth(this->spider.getUnspideredUrl(i)) > min_depth) {
      this->spider.SkipUnspidered(i);
      size--;
    } else {
      i++;
    };
  }
};

void Spider::AddUnspidered(std::string url) {
  this->spider.AddUnspidered(url.c_str());
};

std::vector<std::string> Spider::getOutboundLinks() {
  std::vector<std::string> result;
  std::string url;

  int size = this->spider.get_NumOutboundLinks();
  for (int i = 0; i < size; i++) {
    url = spider.getOutboundLink(i);
    result.push_back(url);
  }

  std::sort(result.begin(), result.end(), orderByDepth);

  return result;
};

std::vector<std::string> Spider::getSpideredUrls() {
  std::vector<std::string> result;
  std::string url;
  int size = this->spider.get_NumSpidered();
  for (int i = 0; i < size; i++) {
    url = this->spider.getSpideredUrl(i);
    result.push_back(url);
  }
  std::sort(result.begin(), result.end(), orderByDepth);

  return result;
};

std::vector<std::string> Spider::getUnspideredUrls() {
  std::vector<std::string> result;
  std::string url;
  int size = this->spider.get_NumUnspidered();
  for (int i = 0; i < size; i++) {
    url = this->spider.getUnspideredUrl(i);
    result.push_back(url);
  }
  std::sort(result.begin(), result.end(), orderByDepth);

  return result;
};

void Spider::printStatus() {
  std::cout << BOLD << "Spider Status" << '\n';
  std::cout << CYAN << "URL: " << BLUE << this->url.getString() << '\n';
  std::cout << CYAN << "Domain: " << BLUE << this->domain.getString() << '\n';
  std::cout << CYAN << "Base Domain: " << BLUE << this->base_domain.getString()
            << '\n';
  std::cout << CYAN << "Depth: " << BLUE << this->depth << '\n';
  std::cout << CYAN << "UniqueID: " << BLUE << this->getUniqueId() << '\n';
  std::string head_html(this->getHtml());
  std::cout << CYAN << "HTML content:\n";
  if (head_html.length() >= 600)
    std::cout << BLUE << head_html.substr(0, 600) << '\n';
  else
    std::cout << BLUE << head_html << '\n';
  std::cout << ENDC << '\n';
};

void Spider::printLinks() {
  auto spidered_links = getSpideredUrls();
  auto unspidered_links = getUnspideredUrls();
  auto out_links = getOutboundLinks();

  std::cout << CYAN << "Spidered Links: " << BLUE << '\n';
  for (auto x : spidered_links)
    std::cout << x << '\n';

  std::cout << CYAN << "Unspidered Links: " << BLUE << '\n';
  for (auto x : unspidered_links)
    std::cout << x << '\n';

  std::cout << CYAN << "\nOutbound Links: " << BLUE << '\n';
  for (auto x : out_links)
    std::cout << x << '\n';

  std::cout << ENDC << '\n';
};

std::string Spider::getUrl() { return std::string(this->url.getString()); };

std::string Spider::getHtml() { return std::string(this->spider.lastHtml()); };

unsigned long long Spider::getUniqueId() {
  std::string string_to_hash(this->url.getString());
  string_to_hash.append(this->html.getString());
  std::hash<std::string> hash_fn;
  unsigned long long id_hash = hash_fn(string_to_hash);

  return id_hash;
};

void Spider::init() {
  // std::vector<std::thread> vec_thr;
  //
  // while (this->crawl()) {
  //   this->removeUnspideredByDepth(MAX_DEPTH);
  //   this->printStatus();
  //   this->printLinks();
  //
  //   for (auto out_link : this->getOutboundLinks()) {
  //     if (countDepth(out_link) <= MAX_DEPTH) {
  //       Spider s_aux(out_link);
  //       vec_thr.emplace_back(&Spider::init, std::move(s_aux));
  //     }
  //   }
  //   for (auto &t : vec_thr) { t.join(); }
  //   sleep(1);
  // };
};
