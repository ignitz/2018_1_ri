#ifndef _FILE_MANAGE_
#define _FILE_MANAGE_
#include <iostream>
#include <fstream>

#include "utils.h"

class FileManager {
protected:
  std::string filename;
  std::fstream file;
  size_t size;

public:
  FileManager (std::string filename);
  virtual ~FileManager ();

  bool open_file();
  bool close_file();
  size_t get_position();
  bool set_position(size_t);
  void read_block(char * buffer, size_t block_size);
};

/**********************************************/
class RWFileManager : public FileManager {
public:
  RWFileManager (std::string filename);
  virtual ~RWFileManager ();

  bool open_file();
  void write_data(char * buffer, size_t block_size);
  void write_data(size_t);
  void write_data(int);
  void write_string(std::string);
};

#endif
