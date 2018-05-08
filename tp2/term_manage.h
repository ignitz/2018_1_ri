#ifndef _TERM_MANAGE_
#define _TERM_MANAGE_

// #include "utils.h"
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

// class TermFile : public RWFileManager {
// private:
//   /* data */
//
// public:
//   TermFile (std::string filename);
//   virtual ~TermFile ();
// };

#endif
