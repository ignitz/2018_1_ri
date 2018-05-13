// #include "block_structure.h"
#include "terms_table.h"
// #include "external_merge_sort.h"
#include <bits/stdc++.h>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << "[nome_da_colecao]" << '\n';
    exit(EXIT_FAILURE);
  }

  std::fstream logFile;
  logFile.open("log_pass2.txt", std::ios::out);
  logFile.close();

  concate_terms();

  return 0;
}

// int main() {
//   auto start = std::chrono::high_resolution_clock::now();
//   std::fstream file(HASHTABLE_FILENAME,
//                     std::ios::in | std::ios::out | std::ios::binary);
//
//   size_t n;
//   file.seekg(0, std::ios::end);
//   n = file.tellg();
//   n /= sizeof(HashBlock);
//   file.seekg(0, std::ios::beg);
//
//   // TermElem aux_term;
//   // std::cout << orderByHashID(aux_term, aux_term) << '\n';
//
//   mergeSort_HashTable (file);
//
//   auto end = std::chrono::high_resolution_clock::now();
//   std::chrono::duration<double> elapsed = end - start;
//   std::cout << "Take " << elapsed.count() << " seconds to sort using MergeSort." << '\n';
//
//   file.seekg(0, std::ios::beg);
//   HashBlock aux_block;
//   char * buffer;
//   std::string word;
//
//   std::fstream f(DUMPTERM_FILENAME, std::ios::in | std::ios::binary);
//   for (size_t i = 0; i < n; i++) {
//     file.read((char *) & aux_block, sizeof(HashBlock));
//     buffer = new char[aux_block.chars_length * sizeof(char)];
//     f.seekg(aux_block.position);
//     f.read(buffer, aux_block.chars_length);
//     std::cout << std::string(buffer, aux_block.chars_length) << '\n';
//     delete[] buffer;
//   }
//
//   f.close();
//
//   file.close();
//
//   return 0;
// }
