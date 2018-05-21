#include "terms.h"

Terms::Terms(const char *collection_filename) {
  this->collection_filename = std::string(collection_filename);

  std::fstream f(DUMPTERM_FILENAME, std::ios::in | std::ios::binary);
  std::fstream g(TERMS_INDEX, std::ios::in | std::ios::binary);
  std::fstream h(HASHTABLE_FILENAME, std::ios::in | std::ios::binary);
  if (!f.is_open() || !g.is_open() || !h.is_open()) {
    std::cerr << "Error on opening HASH_TABLE file!" << '\n';
    exit(EXIT_FAILURE);
  }
  f.close();
  g.close();
  h.close();
}

Terms::~Terms() {}

size_t Terms::get_many_terms() {
  size_t response_size;
  std::fstream f(HASHTABLE_FILENAME, std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    std::cerr << "Error on opening HASH_TABLE file!" << '\n';
    exit(EXIT_FAILURE);
  }

  f.seekg(0, std::ios::end);
  response_size = f.tellg();
  f.close();

  if (response_size % sizeof(HashBlock) != 0) {
    std::cerr << "Wrong file size of HASH_TABLE file!" << '\n';
    exit(EXIT_FAILURE);
  }

  return response_size / sizeof(HashBlock);
};

void Terms::list_all_terms() {
  size_t many_terms = get_many_terms();
  std::fstream hash_file;
  std::fstream term_dump_file;
  // std::fstream term_index_file;

  HashBlock aux_block;
  char *buffer;
  // TermElem aux_term;

  for (size_t i = 0; i < many_terms; i++) {
    hash_file.open(HASHTABLE_FILENAME, std::ios::in | std::ios::binary);
    hash_file.seekg(i * sizeof(HashBlock), std::ios::beg);
    hash_file.read((char *)&aux_block, sizeof(HashBlock));
    {
      term_dump_file.open(DUMPTERM_FILENAME, std::ios::in | std::ios::binary);
      term_dump_file.seekg(aux_block.position, std::ios::beg);
      buffer = new char[aux_block.chars_length];
      term_dump_file.read(buffer, aux_block.chars_length);
      std::cout << std::string(buffer, aux_block.chars_length) << ' '
                << aux_block.freq << '\n';
      delete[] buffer;
      term_dump_file.close();
    }
    hash_file.close();
  }
}

void Terms::boolean_search(std::vector<std::string> terms) {
  TermElem aux_term;
  HashBlock aux_block;
  size_t pointer_to_block;

  // terms with information to cache
  std::vector<InfoTerm> info_terms;

  for (const auto &term : terms) {
    InfoTerm aux_info_term;
    aux_info_term.term = term;
    aux_info_term.pointer = 0;
    pointer_to_block = find_term(term);

    if (pointer_to_block < std::numeric_limits<size_t>::max()) {
      std::fstream f(HASHTABLE_FILENAME, std::ios::in | std::ios::binary);
      f.seekg(pointer_to_block * sizeof(HashBlock), std::ios::beg);
      f.read((char *)&aux_block, sizeof(HashBlock));

      aux_info_term.hash_id = aux_block.hash_id;
      aux_info_term.chars_length = aux_block.chars_length;
      aux_info_term.pointer_to_term = aux_block.pointer_to_term;
      aux_info_term.freq = aux_block.freq;
      f.close();
    } else {
      // term not exist in collection
      std::cout << "Terms not found!" << '\n';
      return;
    }

    info_terms.push_back(std::move(aux_info_term));
  }

  size_t n = info_terms.size();
  size_t remember_last_docID = std::numeric_limits<size_t>::max();

  if (n == 1) {
    std::cout
        << BLUE
        << "List of document's ids that found the term(s): <id, freq in doc>"
        << ENDC << '\n';
    std::fstream f(TERMS_INDEX, std::ios::in | std::ios::binary);
    f.seekg(info_terms[0].pointer_to_term * sizeof(TermElem), std::ios::beg);

    size_t count = 0;

    for (size_t i = 0; i < info_terms[0].freq; i++, count++) {
      f.read((char *)&aux_term, sizeof(TermElem));
      assert(aux_term.hash_id == info_terms[0].hash_id);
      if (remember_last_docID != aux_term.document_id) {
        if (remember_last_docID == std::numeric_limits<size_t>::max()) {
          remember_last_docID = aux_term.document_id;
          continue;
        }
        std::cout << "id " << remember_last_docID << " freq " << count << '\n';
        remember_last_docID = aux_term.document_id;
        count = 0;
      }
    }
    std::cout << "id " << remember_last_docID << " freq " << count << '\n';
    f.close();

  } else if (n > 1) {

    std::cout
        << BLUE
        << "List of document's ids that found the term(s): <id>"
        << ENDC << '\n';

    size_t max_id;
    std::vector<size_t> doc_ids(n, 0);

    bool check, done = false;

    size_t remember_last_docID = std::numeric_limits<size_t>::max();

    while (true) {
      for (size_t i = 0; i < n; i++) {
        // std::cout << BLUE << info_terms[i].pointer << '\t' << info_terms[i].freq << ENDC << '\n';
        if (info_terms[i].pointer >= info_terms[i].freq)
          done = true;
      }

      if (done) break;

      std::fstream f(TERMS_INDEX, std::ios::in | std::ios::binary);

      for (size_t i = 0; i < n; i++) {
        f.seekg((info_terms[i].pointer_to_term + info_terms[i].pointer) *
                    sizeof(TermElem),
                std::ios::beg);
        f.read((char *)&aux_term, sizeof(TermElem));
        doc_ids[i] = aux_term.document_id;
      }

      max_id = 0;
      for (size_t i = 0; i < n; i++) {
        if (max_id < doc_ids[i])
          max_id = doc_ids[i];
      }

      // std::cout << WARNING;
      // std::cout << "max is " << max_id << '\n';
      // for (size_t i = 0; i < n; i++) {
      //   std::cout << doc_ids[i] << '\t';
      // }
      // std::cout << '\n';
      // for (size_t i = 0; i < n; i++)
      //   std::cout << info_terms[i].pointer << '\t';
      // std::cout << ENDC << '\n';

      check = true;
      for (size_t i = 0; i < n; i++) {
        if (doc_ids[i] < max_id) {
          info_terms[i].pointer++;
          check = false;
        }
      }

      if (check) {
        if (remember_last_docID != max_id) {
          std::cout << max_id << '\n';
          remember_last_docID = max_id;
        }

        for (size_t i = 0; i < n; i++)
          info_terms[i].pointer++;
      }

      f.close();
    }
  }
}

size_t Terms::find_term(std::string term_name) {
  size_t many_terms = get_many_terms();
  size_t left, right, term_position;
  std::fstream hash_file;
  std::fstream term_dump_file;

  HashBlock aux_block;
  char *buffer;

  hash_file.open(HASHTABLE_FILENAME, std::ios::in | std::ios::binary);
  term_dump_file.open(DUMPTERM_FILENAME, std::ios::in | std::ios::binary);

  left = 0;
  right = many_terms - 1;

  while (right - left > 1) {
    term_position =
        ((right + left) % 2) ? (right + left) / 2 + 1 : (right + left) / 2;
    // std::cout << term_position << '\t' << left << '\t' << right << '\n';
    hash_file.seekg(term_position * sizeof(HashBlock), std::ios::beg);
    hash_file.read((char *)&aux_block, sizeof(HashBlock));
    term_dump_file.seekg(aux_block.position, std::ios::beg);
    buffer = new char[aux_block.chars_length];
    term_dump_file.read(buffer, aux_block.chars_length);
    // std::cout << std::string(buffer, aux_block.chars_length) << '\n';

    if (std::string(buffer, aux_block.chars_length).compare(term_name) < 0) {
      left = term_position;
    } else if (std::string(buffer, aux_block.chars_length).compare(term_name) >
               0) {
      right = term_position;
    } else {
      // std::cout << "Encounter in position " << term_position << '\n';
      // std::cout << term_name << '\n';
      // std::cout << std::string(buffer, aux_block.chars_length) << '\n';
      hash_file.close();
      term_dump_file.close();
      delete[] buffer;

      return term_position;
    }
    delete[] buffer;
  }

  hash_file.close();
  term_dump_file.close();
  return std::numeric_limits<size_t>::max();
};
