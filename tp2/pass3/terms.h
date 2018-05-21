#ifndef _TERMS_
#define _TERMS_

#include "block_structure.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>    // std::max


struct InfoTerm {
  size_t hash_id;
  size_t chars_length;
  size_t pointer_to_term;
  size_t pointer; // used to boolean_search
  size_t freq;
  std::string term;
};

class Terms {
private:
  std::string collection_filename;

public:
  Terms (const char *);
  virtual ~Terms ();

  size_t get_many_terms();
  void list_all_terms();

  size_t find_term(std::string);

  void boolean_search(std::vector<std::string>);
};

#endif /* end of include guard: _TERMS_ */
