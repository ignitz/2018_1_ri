#ifndef _EXTERNAL_MERGE_SORT_
#define _EXTERNAL_MERGE_SORT_

#define HASHTABLE_FILENAME "hash_table.tbl"
#define DUMPTERM_FILENAME "terms.dump"

#include "block_structure.h"
// #include "terms_table.h"
#include <fstream>
#include <iostream>

void mergeSort_TermElem(std::fstream &);
void mergeSort_HashTable(std::fstream &);

#endif /* end of include guard: _EXTERNAL_MERGE_SORT_ */
