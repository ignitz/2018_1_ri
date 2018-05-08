#ifndef COLORS
#define COLORS

#define CYAN "\033[95m"
#define BLUE "\033[94m"
#define GREEN "\033[92m"
#define WARNING "\033[93m"
#define FAIL "\033[91m"
#define ENDC "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

#endif

#ifndef _UTILS_
#define _UTILS_
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>

#define ILLEGAL_CHARS "\t\n-:.,;/[]{}()?!\"\'©@"

std::string iso_8859_1_to_utf8(std::string &str);
void remove_illegal_chars (std::string & text);
std::vector<std::string> split_string(std::string & text, char split);
#endif
