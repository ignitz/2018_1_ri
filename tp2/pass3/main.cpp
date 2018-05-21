#include <bits/stdc++.h>

#include "block_structure.h"
#include "terms.h"
#include "utils.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " [file collection]" << '\n';
    exit(EXIT_FAILURE);
  }

  std::string str_input;

  Terms term_manage(argv[1]);

  std::cout << BOLD << "Search terms (Boolean mode)" << ENDC << '\n'
            << CYAN << "type \"/help\" for more info." << ENDC << '\n';

  while (true) {
    std::cout << "\n> ";
    std::getline(std::cin, str_input);

    auto terms = split_string(str_input, ' ');

    if (terms.size() == 1) {
      if (terms[0][0] == '/') {
        if (terms[0].compare("/quit") == 0) {
          break;
        } else if (terms[0].compare("/list") == 0) {
          std::cout << "Listing available terms:" << '\n';
          term_manage.list_all_terms();
          continue;
        } else if (terms[0].compare("/help") == 0) {
          std::cout << GREEN
                    << "Just type your terms separated with ' ' to search the "
                       "available documents\n  Example: apoiam instagram\n\n"
                    << "Commands:\n"
                    << "/list\n  print all terms available in hashtable file"
                    << '\n'
                    << "/search <term> or /s <term>\n  Search if term exist in "
                       "hash table"
                    << '\n'
                    << "/quit\n  Close program" << ENDC << '\n';
          continue;
        } else {
          std::cout << "Invalid command" << '\n';
          continue;
        }
      }
    } else if (terms.size() == 2) {
      if (terms[0][0] == '/') {
        if (terms[0].compare("/s") == 0 || terms[0].compare("/search") == 0) {
          // term_manage.find_term(terms[1]);
          size_t found = term_manage.find_term(terms[1]);
          if (found < std::numeric_limits<size_t>::max()) {
            std::cout << "Found term in position " << found << '\n';
          } else {
            std::cout << "Not found!" << '\n';
          }
          continue;
        }
        else {
          std::cout << "Invalid command" << '\n';
          continue;
        }
      }
    }

    term_manage.boolean_search(terms);
  }

  return 0;
}
