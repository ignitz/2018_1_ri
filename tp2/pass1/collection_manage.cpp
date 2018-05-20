#include "collection_manage.h"

CollectionManager::CollectionManager(std::string filename)
    : FileManager(filename) {
  std::string index_file_name = filename + ".index";
  std::string urlList_file_name = filename + ".urllist";

  pCollection = new PointerCollection(index_file_name, urlList_file_name);
  pTermhash = new TermHash('s');
  logFile.open("log.txt", std::ios::out);

  std::string string_log;
  string_log = "/**********************/\nInitiating collection of terms\n";
  string_log += "Collection: " + filename +
                "\n\tindex file: " + index_file_name +
                "\n\turl list file: " + urlList_file_name + "\n";
  logFile << string_log;
  logFile.close();
}

CollectionManager::~CollectionManager() {
  // logFile.close();
  delete pCollection;
  delete pTermhash;
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
  std::chrono::duration<double> elapsed;

  std::string url, content;

  size_t document_id = 0;
  size_t last_many_terms = 0;
  // TODO: make save state
  auto start_global_time = std::chrono::high_resolution_clock::now();
  while (!eof()) {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::cout << BOLD << "Progress: " << ' '
              << (100 * get_position() / get_size()) << "%\t" << BLUE
              << "Position: " << get_position()
              << "\tDocument ID: " << document_id << ENDC << '\n';
    url = read_url();
    position = get_position();
    content = read_content();
    pCollection->insert_doc(url, position);
    std::vector<Term> terms = get_terms(content, document_id);
    for (auto &each_term : terms) {
      this->pTermhash->add_term(each_term);
    }

    auto finish_time = std::chrono::high_resolution_clock::now();
    elapsed = finish_time - start_time;


    std::string string_log =
        std::to_string(elapsed.count()) + " seconds to read document " +
        std::to_string(document_id) + '\n' + '\t' +
        std::to_string(this->pTermhash->get_many_terms() - last_many_terms) +
        " terms added.\n";

    last_many_terms = this->pTermhash->get_many_terms();
    
    std::cout << GREEN << string_log << ENDC << '\n';

    this->write_to_log(string_log);

    document_id++;
  }
  auto finish_global_time = std::chrono::high_resolution_clock::now();
  elapsed = finish_global_time - start_global_time;
  std::string result_string =
      "\nCollection read takes:\n" + std::to_string(elapsed.count()) +
      " seconds to finish\n" + "There are " +
      std::to_string(this->pTermhash->get_many_terms()) + " terms.\n";
  std::cout << GREEN << result_string << ENDC << '\n';
  this->write_to_log(result_string);
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
    std::vector<Term> terms = get_terms(content, document_id);
    for (auto &each_term : terms) {
      this->pTermhash->add_term(each_term);
    }
  }
  return true;
}

void CollectionManager::print_all_terms() {
  size_t many_terms = this->pTermhash->get_many_terms();

  for (size_t i = 0; i < many_terms; i++) {
    std::cout << this->get_term_by_id(i) << '\n';
  }
}

size_t CollectionManager::get_position_url(size_t id) {
  return std::move(this->pCollection->get_position_url(id));
}

size_t CollectionManager::get_position_id(size_t id) {
  return std::move(this->pCollection->get_position_id(id));
}

std::string CollectionManager::get_term_by_id(size_t id) {
  return std::move(this->pTermhash->get_term_by_id(id));
};

void CollectionManager::write_to_log(std::string string_log) {
  logFile.open("log.txt", std::ios::app);
  logFile << string_log << '\n';
  logFile.close();
};

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
