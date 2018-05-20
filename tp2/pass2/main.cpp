// #include "block_structure.h"
#include "terms_table.h"
// #include "external_merge_sort.h"
#include <bits/stdc++.h>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << "[collection_filename]" << '\n';
    exit(EXIT_FAILURE);
  }

  std::fstream logFile;
  logFile.open("log_pass2.txt", std::ios::out);
  logFile.close();

  concate_terms();

  return 0;
}
