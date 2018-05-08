#include "collection_manage.h"

CollectionManager::CollectionManager(std::string filename)
    : FileManager(filename) {
  std::string index_file_name = filename + ".index";
  std::string urlList_file_name = filename + ".urllist";

  pCollection = new PointerCollection(index_file_name, urlList_file_name);
}

CollectionManager::~CollectionManager() {
  delete pCollection;
  this->FileManager::~FileManager();
}

bool CollectionManager::open_file() {
  this->FileManager::open_file();

  if (size > 3) {
    for (size_t i = 0; i < 3; i++) {
      if (file.get() != '|') {
        std::cout << "File " << filename << " with bad format!" << '\n';
        exit(EXIT_FAILURE);
      }
    }
  }
  return true;
}

std::string CollectionManager::read_block(size_t block_size) {
  char *buffer = new char[block_size];
  size_t position = file.tellg();
  file.seekg(position - block_size - 1);
  file.read(buffer, block_size);
  std::string response = std::string(buffer, block_size);
  delete[] buffer;
  return std::move(response);
}

std::string CollectionManager::read_url() {
  size_t block_size = 0;
  while (file.get() != int('|')) {
    if (file.eof()) {
      std::cout << FAIL << "Unexpected EOF!" << ENDC << '\n';
      exit(EXIT_FAILURE);
    }
    block_size++;
  }

  std::string response = this->read_block(block_size);
  file.get();
  return std::move(response);
}

std::string CollectionManager::read_content() {
  std::string content;
  size_t block_size = 0;
  char c;

  while (file.get() != int('|')) {
    if (file.eof()) {
      std::cout << FAIL << "Unexpected EOF!" << ENDC << '\n';
      exit(EXIT_FAILURE);
    }
    block_size++;
  }

  content = this->read_block(block_size);

  for (size_t i = 0; i < 3; i++) {
    file.get(c);
    if (file.eof()) {
      std::cout << FAIL << "Unexpected EOF!" << ENDC << '\n';
      exit(EXIT_FAILURE);
    }
    if (c != '|') {
      std::cout << FAIL << "File " << filename << " with bad format!" << ENDC
                << '\n';
      exit(EXIT_FAILURE);
    }
  }

  return std::move(content);
}

bool CollectionManager::eof() {
  size_t position = file.tellg();
  return file.eof() || (position >= size - 1);
}

bool CollectionManager::read_file() {
  if (!file.is_open()) {
    std::cout << FAIL << "File not opened" << ENDC << '\n';
    exit(EXIT_FAILURE);
  }

  size_t position;

  std::string url, content;

  while (!eof()) {
    url = read_url();
    position = file.tellg();
    content = read_content();
    pCollection->insert_doc(url, position);
  }
  return true;
}

/*
 * DEBUG purpose
 */
bool CollectionManager::read_exact_pages(size_t many_pages) {
  if (!file.is_open()) {
    std::cout << FAIL << "File not opened" << ENDC << '\n';
    exit(EXIT_FAILURE);
  }

  size_t position;

  std::string url, content;

  for (size_t document_id = 0; document_id < many_pages && !eof();
       document_id++) {
    url = read_url();
    position = file.tellg();
    content = read_content();
    pCollection->insert_doc(url, position);
    get_terms(content, document_id);
    // std::cout << get_cleanText(content) << '\n';
    // std::cout << content << '\n';
  }
  return true;
}

size_t CollectionManager::get_position_url(size_t id) {
  return std::move(this->pCollection->get_position_url(id));
}

size_t CollectionManager::get_position_id(size_t id) {
  return std::move(this->pCollection->get_position_id(id));
}

/********************************************************/
PointerCollection::PointerCollection(std::string docIDs_name,
                                     std::string urlList_name) {
  docIDs = new RWFileManager(docIDs_name);
  docIDs->open_file();
  urlList = new RWFileManager(urlList_name);
  urlList->open_file();
}

PointerCollection::~PointerCollection() {
  delete docIDs;
  delete urlList;
}

void PointerCollection::insert_doc(std::string url, size_t doc_position) {
  size_t position;
  position = urlList->get_position();
  docIDs->write_data(position);
  docIDs->write_data(doc_position);
  urlList->write_string(url);
}

size_t PointerCollection::get_position_url(size_t id) {
  size_t response, position;
  position = id * 2 * sizeof(size_t);
  docIDs->set_position(position);
  docIDs->read_block((char *)&response, sizeof(size_t));
  return std::move(response);
}

size_t PointerCollection::get_position_id(size_t id) {
  size_t response, position;
  position = id * 2 * sizeof(size_t) + sizeof(size_t);
  docIDs->set_position(position);
  docIDs->read_block((char *)&response, sizeof(size_t));
  return std::move(response);
}

/********************************************************/
int main() {

  CollectionManager manage("html_pages.txt");
  manage.open_file();
  // manage.read_file();
  manage.read_exact_pages(100);

  return 0;
}
