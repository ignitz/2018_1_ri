// Colorir letras. Testado apenas no Linux.
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

#ifndef UTILS
#define UTILS
#include <CkUrl.h>
#include <CkSpider.h>
#include <string>
#include <algorithm>

// Count the depth of URL
int countDepth(std::string);
std::string getBaseDomain(std::string);
bool check_if_not_BR(std::string);
#endif
