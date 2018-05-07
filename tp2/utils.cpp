#include "utils.h"

std::string iso_8859_1_to_utf8(std::string &str) {
  using namespace std;
  string strOut;
  for (string::iterator it = str.begin(); it != str.end(); ++it) {
    uint8_t ch = *it;
    if (ch < 0x80) {
      strOut.push_back(ch);
    } else {
      strOut.push_back(0xc0 | ch >> 6);
      strOut.push_back(0x80 | (ch & 0x3f));
    }
  }
  return strOut;
}

// need boost library
std::vector<std::string> split_string(std::string text) {
  std::vector<std::string> results;
  std::string::iterator it;

  std::string illegalChars = "\t\n-:.,;/[]{}()?!\"\'";
  for (it = text.begin(); it < text.end(); ++it) {
    bool found = illegalChars.find(*it) != std::string::npos;
    if (found) {
      *it = ' ';
    }
  }

  boost::split(results, text, [](char c) { return c == ' '; });
  return std::move(results);
}
