#include "file_manage.h"

FileManager::FileManager(std::string filename) { this->filename = filename; }

FileManager::~FileManager() { file.close(); }

bool FileManager::open_file() {
  file.open(filename, std::ios::in | std::ios::binary);

  if (!file.is_open()) {
    std::cout << FAIL << "Error in trying to open file" << ENDC << '\n';
    return false;
  }

  file.seekg(0, std::ios::end);
  size = file.tellg();
  file.seekg(0, std::ios::beg);
  return true;
}

bool FileManager::close_file() {
  file.close();
  return file.is_open();
}

size_t FileManager::get_position() {
  size_t position = file.tellg();
  return position;
}

bool FileManager::set_position(size_t position) {
  file.seekg(position);
  size_t check = file.tellg();
  return check <= size ? true : false;
}

void FileManager::read_block(char *buffer, size_t block_size) {
  file.read(buffer, block_size);
}

/**********************************************/
RWFileManager::RWFileManager(std::string filename) : FileManager(filename) {}

RWFileManager::~RWFileManager() { file.close(); }

bool RWFileManager::open_file() {
  file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    file.close();
    file.open(filename, std::ios::out | std::ios::binary);
  }

  return file.is_open();
}

void RWFileManager::write_data(char *buffer, size_t block_size) {
  file.write(buffer, block_size);
}

void RWFileManager::write_data(size_t buffer) {
  this->write_data((char *)&buffer, sizeof(size_t));
}

void RWFileManager::write_data(int buffer) {
  this->write_data((char *)&buffer, sizeof(int));
}

void RWFileManager::write_string(std::string string) {
  this->file << string << '\n';
}
