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

void remove_illegal_chars (std::string & text) {
  std::string::iterator it;

  std::string illegalChars = ILLEGAL_CHARS;
  for (it = text.begin(); it < text.end(); ++it) {
    bool found = illegalChars.find(*it) != std::string::npos;
    if (found) {
      *it = ' ';
    }
  }
}

// need boost library
std::vector<std::string> split_string(std::string & text, char split) {
  std::vector<std::string> results;
  boost::split(results, text, [&split](char c) { return c == split; });
  return std::move(results);
}
