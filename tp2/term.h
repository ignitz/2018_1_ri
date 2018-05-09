#ifndef _TERM_
#define _TERM_

#include <iostream>

typedef struct {
  size_t offset;
  size_t length;
  std::string phrase;
} Phrase;

typedef struct {
  size_t document_id;
  size_t position;
  std::string term;
} Term;

#endif /* end of include guard: _TERM_ */
