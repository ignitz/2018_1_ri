#ifndef _BLK_STRUCT_
#define _BLK_STRUCT_
#include <iostream>

struct HashBlock {
  size_t hash_id;
  size_t position;
  size_t chars_length;
  size_t pointer_to_term;
  size_t freq;
};

struct TermElem {
  size_t hash_id;
  size_t document_id;
  size_t position;
};

#endif /* end of include guard: _BLK_STRUCT_ */
