#include "term_manage.h"

TermHash::TermHash(char mode) {
  switch (mode) {
  case 's':
    if (!check_if_file_exist(HASHTABLE_FILENAME)) {
      std::fstream f(HASHTABLE_FILENAME, std::ios::out | std::ios::binary);
      f.close();
    }
    hash_table.open(HASHTABLE_FILENAME, std::ios::in | std::ios::out | std::ios::binary);

    if (!check_if_file_exist(DUMPTERM_FILENAME)) {
      std::fstream f(DUMPTERM_FILENAME, std::ios::out | std::ios::binary);
      f.close();
    }
    terms_dump.open(DUMPTERM_FILENAME, std::ios::in | std::ios::out | std::ios::binary);

    break;
  case 'c':
    // NOT YET IMPLEMENTED
    exit(EXIT_FAILURE);
    break;
  default:
    std::cerr
        << FAIL << "Wrong mode passed by params: " << mode << '\n'
        << "\tAvailable is 'c' to continue or 's' to start from beggining!"
        << ENDC << '\n';
    exit(EXIT_FAILURE);
  }

  if (!hash_table.is_open()) {
    std::cerr << FAIL << "Error on opening file " << HASHTABLE_FILENAME << ENDC
              << '\n';
    exit(EXIT_FAILURE);
  }
  if (!terms_dump.is_open()) {
    std::cerr << FAIL << "Error on opening file " << DUMPTERM_FILENAME << ENDC
              << '\n';
    exit(EXIT_FAILURE);
  }
}

TermHash::~TermHash() { hash_table.close(); }

std::string TermHash::get_term_by_id(size_t id) {

  HashBlock aux_block;

  hash_table.seekg(0, std::ios::end);
  size_t hash_length = hash_table.tellg();
  hash_table.seekg(0, std::ios::beg);

  hash_length /= (sizeof(size_t) * 3);

  size_t i = 0;

  while (i < hash_length) {
    hash_table.read((char *)&aux_block, sizeof(HashBlock));
    if (aux_block.hash_id == id)
      break;
    i++;
  }

  // block not found
  if (i == hash_length)
    return std::move(std::string(""));

  terms_dump.seekg(aux_block.position, std::ios::beg);

  char *buffer = new char[aux_block.size*sizeof(char)];
  terms_dump.read(buffer, aux_block.size*sizeof(char));
  std::string response(buffer, aux_block.size);
  delete[] buffer;

  return std::move(response);
}

void TermHash::add_term(Term &term) {
  // check if term exists
  std::string strTerm = term.term;

  HashBlock aux_block;

  // TODO: o(n)... weell need binary search?
  hash_table.seekg(0, std::ios::end);
  size_t hash_length = hash_table.tellg();
  hash_table.seekg(0, std::ios::beg);

  hash_length /= sizeof(HashBlock);

  terms_dump.seekg(0, std::ios::beg);

  for (size_t i = 0; i < hash_length; i++) {
    hash_table.read((char *) &aux_block, sizeof(HashBlock));
    char *buffer = new char[aux_block.size*sizeof(char)];
    terms_dump.read(buffer, aux_block.size*sizeof(char));
    // if exists then insert in term files
    if (std::string(buffer, aux_block.size).compare(strTerm) == 0) {
      aux_file.open("terms/" +
                        std::to_string((aux_block.hash_id / MANY_ON_DAT_BLOCK)),
                    std::ios::app | std::ios::binary);
      if (!aux_file.is_open()) {
        std::cerr << "corre negada" << '\n';
        exit(EXIT_FAILURE);
      }
      aux_file.write((char *)&aux_block.hash_id, sizeof(size_t));
      aux_file.write((char *)&term.document_id, sizeof(size_t));
      aux_file.write((char *)&term.position, sizeof(size_t));
      aux_file.close();
      return;
    };

    delete[] buffer;
  }

  // if not then create file and insert in hashtable and cat termdumpfile
  // terms_dump.seekg(0, std::ios::end);
  size_t position = terms_dump.tellg();
  aux_block.hash_id = hash_length;
  aux_block.position = position;
  aux_block.size = term.term.length();
  hash_table.write((char *) & aux_block, sizeof(HashBlock));

  terms_dump.write(term.term.c_str(), aux_block.size);

  aux_file.open("terms/" +
                    std::to_string((aux_block.hash_id / MANY_ON_DAT_BLOCK)),
                std::ios::app | std::ios::binary);
  if (!aux_file.is_open()) {
    std::cerr << "corre negada" << '\n';
    exit(EXIT_FAILURE);
  }
  aux_file.write((char *)&aux_block.hash_id, sizeof(size_t));
  aux_file.write((char *)&term.document_id, sizeof(size_t));
  aux_file.write((char *)&term.position, sizeof(size_t));
  aux_file.close();

  return;
}

size_t TermHash::get_many_terms() {
  size_t position = hash_table.tellg();
  hash_table.seekg(0, std::ios::end);
  size_t hash_length = hash_table.tellg();
  hash_table.seekg(position, std::ios::beg);
  hash_length /= sizeof(HashBlock);
  return hash_length;
}

// int main(int argc, char const *argv[]) {
//   TermHash('s');
//
//   std::cout << argc << ' ' << argv[0] << '\n';
//   std::cout << "/* message */" << '\n';
//   return 0;
// }
