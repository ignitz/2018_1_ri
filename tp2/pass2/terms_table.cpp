#include "terms_table.h"

bool orderByHashID_MEMORY(TermElem x, TermElem y) { return (x.hash_id < y.hash_id); };

void order_in_memory(std::fstream & file, std::fstream & file_order) {
  std::vector<TermElem> termsFreq;
  TermElem aux_term;

  file.seekg(0, std::ios::end);
  size_t size_file = file.tellg();
  if ((size_file % sizeof(TermElem)) != 0 ) {
    std::cerr << "Wrong file size" << '\n';
    exit(EXIT_FAILURE);
  }
  size_file /= sizeof(TermElem);
  file.seekg(0, std::ios::beg);

  for (size_t i = 0; i < size_file; i++) {
    file.read((char *)&aux_term, sizeof(TermElem));
    termsFreq.push_back(aux_term);
  }

  std::stable_sort(termsFreq.begin(), termsFreq.end(), orderByHashID_MEMORY);

  for (size_t i = 0; i < size_file; i++) {
    file_order.write((char *)&termsFreq[i], sizeof(TermElem));
  }
}

void test() {
  size_t many_files = 0;
  std::fstream file;
  std::fstream big_file("BIGFILE", std::ios::out | std::ios::binary);
  // check if open

  while (true) {
    file.open(std::string("terms/" + std::to_string(many_files)));
    if (!file.is_open()) {
      break;
    }

    order_in_memory(file, big_file);
    file.close();
    many_files++;
  }

  file.close();
  big_file.close();

  std::cout << "There are " << many_files << " files!" << '\n';
}
