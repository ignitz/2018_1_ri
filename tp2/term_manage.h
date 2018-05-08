#ifndef _TERM_MANAGE_
#define _TERM_MANAGE_

#include <vector>

#include "gumbo.h"
#include "utils.h"

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

std::string get_cleanText(std::string html);
void test_gumbo(std::string, size_t);

#endif
