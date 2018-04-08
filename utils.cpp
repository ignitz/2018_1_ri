#include "utils.h"

int countDepth(std::string url) {
  CkUrl url_util;
  url_util.ParseUrl(url.c_str());
  std::string host(url_util.host());

  int points = std::count(host.begin(), host.end(), '.');
  int host_found = url.find(host) + host.length();
  int dashes = std::count(url.begin() + host_found, url.end(), '/');

  // found if have www and subtract 1 depth
  int www_found = url.find("//www.");

  int result = points + dashes;
  if (www_found > 0 && www_found <= int(host.length() + 5) && dashes > 0)
    result--;

  return result;
};
