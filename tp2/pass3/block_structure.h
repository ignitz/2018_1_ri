#ifndef _BLK_STRUCT_
#define _BLK_STRUCT_
#include <iostream>

#define HASHTABLE_FILENAME "hash_table.tbl"
#define DUMPTERM_FILENAME "terms.dump"
#define TERMS_INDEX "terms.index"

struct HashBlock {
  size_t hash_id;  // unique id of term
  size_t position; // position to char dump
  size_t chars_length; // lenth of term
  size_t pointer_to_term; // ponteiro para o block do termo
  size_t freq; // freq of term on all documents
};

struct TermElem {
  size_t hash_id; // self
  size_t document_id; // id do documento encontrado
  size_t position; // ponteiro para o termo no documento
};

#endif /* end of include guard: _BLK_STRUCT_ */
