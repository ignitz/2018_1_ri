#ifndef _TERM_MANAGE_
#define _TERM_MANAGE_

#define HASHTABLE_FILENAME "hash_table.tbl"
#define DUMPTERM_FILENAME "terms.dump"
#define MANY_ON_DAT_BLOCK 100

// #include "utils.h"
#include <iostream>
#include <fstream>

#include "utils.h"
#include "term.h"

struct HashBlock {
  size_t hash_id;
  size_t position;
  size_t size;
};

class TermHash {
private:
  std::fstream hash_table;
  std::fstream terms_dump;
  std::fstream aux_file;

  // Hash Table
  size_t hash_id, position, size;

public:
  // mode:
  // 'c' --> continue, continue file from saved state
  // 's' --> start. delete files and continue
  TermHash (char mode);
  virtual ~TermHash ();

  std::string get_term_by_id(size_t id);
  void add_term(Term &);
  size_t get_many_terms();
};

#endif
