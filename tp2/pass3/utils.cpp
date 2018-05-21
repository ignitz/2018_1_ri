#include "utils.h"

// need boost library
std::vector<std::string> split_string(std::string &text, char split) {
  std::vector<std::string> results;
  boost::split(results, text, [&split](char c) { return c == split; });
  return std::move(results);
}
