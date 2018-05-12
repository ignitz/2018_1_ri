#ifndef _COL_MANAGE_
#define _COL_MANAGE_
#include <iostream>
#include <chrono>

#include "file_manage.h"
#include "term_manage.h"
#include "term.h"
#include "utils.h"

class PointerCollection {
private:
  RWFileManager *docIDs;
  RWFileManager *urlList;

public:
  PointerCollection(std::string docIDs_name, std::string urlList_name);
  virtual ~PointerCollection();
  void insert_doc(std::string url, size_t doc_position);
  size_t get_position_id(size_t id);
  size_t get_position_url(size_t id);
};

/**********************************************/
class CollectionManager : public FileManager {
private:
  PointerCollection *pCollection;
  TermHash *pTermhash;
  std::fstream logFile;

public:
  CollectionManager(std::string filename);
  virtual ~CollectionManager();

  bool open_file();
  std::string read_block(size_t block_size);
  std::string read_url();
  std::string read_content();
  bool eof();
  bool read_file();
  bool read_exact_pages(size_t many_pages);
  void print_all_terms();
  size_t get_position_id(size_t id);
  size_t get_position_url(size_t id);
  std::string get_term_by_id(size_t id);
  void write_to_log(std::string);
};

#endif
