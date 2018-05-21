#include "terms_table.h"

void write_single_term_to_unique_file(std::fstream & file_term) {
  std::fstream terms_index(TERMS_INDEX, std::ios::out | std::ios::app | std::ios::binary);

  TermElem term_element;
  size_t many_terms;

  size_t last_hash_id, last_position, count;

  file_term.seekg(0, std::ios::end);
  many_terms = file_term.tellg();
  many_terms /= sizeof(TermElem);
  file_term.seekg(0, std::ios::beg);

  last_position = terms_index.tellg();
  last_position /= sizeof(TermElem);

  file_term.read((char *) & term_element, sizeof(TermElem));
  terms_index.write((char *) & term_element, sizeof(TermElem));

  last_hash_id = term_element.hash_id;
  count = 1;

  for (size_t i = 1; i < many_terms; i++) {

    file_term.read((char *) & term_element, sizeof(TermElem));
    terms_index.write((char *) & term_element, sizeof(TermElem));

    if (last_hash_id == term_element.hash_id) {
      count++;
    }
    else {
      std::fstream hash_table(HASHTABLE_FILENAME, std::ios::in | std::ios::out | std::ios::binary);
      hash_table.seekg(last_hash_id * sizeof(HashBlock) + 3*sizeof(size_t));
      hash_table.write((char*) & last_position, sizeof(size_t));
      hash_table.write((char*) & count, sizeof(size_t));

      hash_table.close();

      last_hash_id = term_element.hash_id;
      last_position = last_position + count;
      count = 1;
    }
  }

  std::fstream hash_table(HASHTABLE_FILENAME, std::ios::in | std::ios::out | std::ios::binary);
  hash_table.seekg(last_hash_id * sizeof(HashBlock) + 3*sizeof(size_t));
  hash_table.write((char*) & last_position, sizeof(size_t));
  hash_table.write((char*) & count, sizeof(size_t));
  hash_table.close();

  terms_index.close();
}

void concate_terms(){

  auto concat_start_time = std::chrono::high_resolution_clock::now();

  std::fstream terms_index(TERMS_INDEX, std::ios::out | std::ios::binary);
  terms_index.close();

  std::chrono::duration<double> elapsed;

  std::fstream file_term;

  size_t count = 0;

  while (true) {
    auto start_time = std::chrono::high_resolution_clock::now();
    file_term.open("terms/" + std::to_string(count), std::ios::in | std::ios::out | std::ios::binary);
    if (!file_term.is_open()) {
      break;
    }

    std::cout << "Sorting file " << count << '\n';
    mergeSort_TermElem(file_term);
    auto mid_time = std::chrono::high_resolution_clock::now();
    elapsed = mid_time - start_time;
    std::cout << "\telapsed time to sort " << std::to_string(elapsed.count()) << " seconds\n";

    std::cout << "Writing file " << count << '\n';
    write_single_term_to_unique_file(file_term);
    file_term.close();
    auto end_time = std::chrono::high_resolution_clock::now();
    elapsed = end_time - mid_time;
    std::cout << "\telapsed time to write " << std::to_string(elapsed.count()) << " seconds\n";

    std::remove(std::string("terms/" + std::to_string(count)).c_str());
    count++;
  }

  auto concat_end_time = std::chrono::high_resolution_clock::now();
  elapsed = concat_end_time - concat_start_time;
  std::cout << "Total elapsed to join terms files was " << std::to_string(elapsed.count()) << " seconds\n";

  std::fstream hash_table(HASHTABLE_FILENAME, std::ios::in | std::ios::out | std::ios::binary);
  auto sort_hash_start_time = std::chrono::high_resolution_clock::now();
  mergeSort_HashTable(hash_table);
  auto sort_hash_end_time = std::chrono::high_resolution_clock::now();
  elapsed = sort_hash_end_time - sort_hash_start_time;
  std::cout << "Time elapsed to sort hash table file was " << std::to_string(elapsed.count()) << " seconds\n";
  hash_table.close();
};
