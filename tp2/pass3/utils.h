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

#define ILLEGAL_CHARS "\t\n-:.,;/[]{}()?!\"\'©@&_*$#“”+-=<>%"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <vector>

std::vector<std::string> split_string(std::string &text, char split);

#endif
