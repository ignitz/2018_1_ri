#include "external_merge_sort.h"
// #include <chrono>

template <class BlockType>
void merge(std::fstream &file, size_t l, size_t m, size_t r, bool (*)(BlockType&, BlockType&));

bool orderByHashID(TermElem & aux_1, TermElem & aux_2) {
  return (aux_1.hash_id <= aux_2.hash_id);
}

bool orderByName(HashBlock & aux_1, HashBlock & aux_2) {
  char * buffer;

  std::fstream f(DUMPTERM_FILENAME, std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    std::cerr << "Error on opening HASH_TABLE file!" << '\n';
    exit(EXIT_FAILURE);
  }

  buffer = new char[aux_1.chars_length * sizeof(char)];

  f.seekg(aux_1.position);
  f.read(buffer, aux_1.chars_length);
  std::string string1(buffer, aux_1.chars_length);

  delete[] buffer;

  buffer = new char[aux_2.chars_length * sizeof(char)];

  f.seekg(aux_2.position);
  f.read(buffer, aux_2.chars_length);
  std::string string2(buffer, aux_2.chars_length);

  delete[] buffer;

  f.close();
  // std::cout << "Compare " << string1 << " with " << string2 << ' ' <<
  // string1.compare(string2) << '\n';
  return (string1.compare(string2) <= 0);
}

int min(size_t x, size_t y) { return (x < y) ? x : y; }

void mergeSort_TermElem(std::fstream &file) {
  size_t n;
  file.seekg(0, std::ios::end);
  n = file.tellg();
  if ((n % sizeof(TermElem)) != 0) {
    std::cerr << "Wrong file size!" << '\n';
    exit(EXIT_FAILURE);
  }
  n /= sizeof(TermElem);
  file.seekg(0, std::ios::beg);

  {
    std::fstream L("l.temp", std::ios::out | std::ios::binary);
    std::fstream R("r.temp", std::ios::out | std::ios::binary);
    L.close();
    R.close();
  }

  for (size_t curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size) {
    for (size_t left_start = 0; left_start < n - 1;
         left_start += 2 * curr_size) {

      size_t mid = left_start + curr_size - 1;
      size_t right_end = min(left_start + 2 * curr_size - 1, n - 1);
      merge<TermElem>(file, left_start, mid, right_end, orderByHashID);
    }
  }

  std::remove("l.temp");
  std::remove("r.temp");
}

void mergeSort_HashTable(std::fstream &file) {
  size_t n;
  file.seekg(0, std::ios::end);
  n = file.tellg();
  if ((n % sizeof(HashBlock)) != 0) {
    std::cerr << "Wrong file size!" << '\n';
    exit(EXIT_FAILURE);
  }
  n /= sizeof(HashBlock);
  file.seekg(0, std::ios::beg);

  {
    std::fstream L("l.temp", std::ios::out | std::ios::binary);
    std::fstream R("r.temp", std::ios::out | std::ios::binary);
    L.close();
    R.close();
  }

  for (size_t curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size) {
    for (size_t left_start = 0; left_start < n - 1;
         left_start += 2 * curr_size) {

      size_t mid = left_start + curr_size - 1;
      size_t right_end = min(left_start + 2 * curr_size - 1, n - 1);
      merge<HashBlock>(file, left_start, mid, right_end, orderByName);
    }
  }

  std::remove("l.temp");
  std::remove("r.temp");
}

template <class BlockType>
void merge(std::fstream &file, size_t l, size_t m, size_t r, bool (* orderby)(BlockType&, BlockType&)) {
  std::fstream L;
  std::fstream R;

  BlockType aux_term;
  BlockType aux_1;
  BlockType aux_2;

  // assert(l <= m);
  // assert(l < r);
  // assert(m <= r);
  if (m > r)
    m = r;

  size_t i, j, k;
  size_t n1 = m - l + 1;
  size_t n2 = r - m;

  L.open("l.temp", std::ios::in | std::ios::out | std::ios::binary);
  R.open("r.temp", std::ios::in | std::ios::out | std::ios::binary);
  L.seekg(0, std::ios::beg);
  R.seekg(0, std::ios::beg);

  file.seekg(l * sizeof(BlockType));
  for (i = 0; i < n1; i++) {
    file.read((char *)&aux_term, sizeof(BlockType));
    L.write((char *)&aux_term, sizeof(BlockType));
  }

  file.seekg((m + 1) * sizeof(BlockType));
  for (j = 0; j < n2; j++) {
    file.read((char *)&aux_term, sizeof(BlockType));
    R.write((char *)&aux_term, sizeof(BlockType));
  }

  i = 0;
  j = 0;
  k = l;

  file.seekg(k * sizeof(BlockType));
  while (i < n1 && j < n2) {
    L.seekg(i * sizeof(BlockType));
    R.seekg(j * sizeof(BlockType));
    L.read((char *)&aux_1, sizeof(BlockType));
    R.read((char *)&aux_2, sizeof(BlockType));
    if (orderby(aux_1, aux_2)) {
      file.write((char *)&aux_1, sizeof(BlockType));
      i++;
    } else {
      file.write((char *)&aux_2, sizeof(BlockType));
      j++;
    }
    k++;
  }

  L.seekg(i * sizeof(BlockType));
  while (i < n1) {
    L.read((char *)&aux_term, sizeof(BlockType));
    file.write((char *)&aux_term, sizeof(BlockType));
    i++;
  }

  R.seekg(j * sizeof(BlockType));
  while (j < n2) {
    R.read((char *)&aux_term, sizeof(BlockType));
    file.write((char *)&aux_term, sizeof(BlockType));
    j++;
  }

  L.close();
  R.close();
}
