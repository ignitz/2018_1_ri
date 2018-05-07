#ifndef _TERM_
#define _TERM_
#include "term.h"
#include <iostream>

class Term {
private:
  std::string word;
  size_t count;

public:
  Term(std::string word) {
    this->word = word;
    this->count = 1;
  };
  virtual ~Term(){};
  std::string get_word() { return word; };
  size_t get_count() { return count; };
  void plus_one() { this->count++; };
};

bool orderByFreq(Term x, Term y, bool invert = false) {
  return invert ? (x.get_count() < y.get_count()) : (x.get_count() >= y.get_count());
};

#endif
