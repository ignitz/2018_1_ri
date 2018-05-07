#ifndef _COL_MANAGE_
#define _COL_MANAGE_
#include <iostream>
#include <vector>

#include "file_manage.h"
#include "utils.h"

class PointerCollection {
private:
  RWFileManager * docIDs;
  RWFileManager * urlList;

public:
  PointerCollection (std::string docIDs_name, std::string urlList_name);
  virtual ~PointerCollection ();
  void insert_doc(std::string url, size_t doc_position);
};

/**********************************************/
class CollectionManager : public FileManager {
private:
  PointerCollection * pCollection;
public:
  CollectionManager (std::string filename);
  virtual ~CollectionManager ();

  bool open_file();
  std::string read_block(size_t block_size);
  std::string read_url();
  std::string read_content();
  bool eof();
  bool read_file();
};

#endif
