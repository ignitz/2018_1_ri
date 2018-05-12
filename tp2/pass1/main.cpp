#include "collection_manage.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <collection_file>\n";
    exit(EXIT_FAILURE);
  }
  CollectionManager manage(argv[1]);
  manage.open_file();
  manage.read_file();
  // manage.read_exact_pages(100);
  // manage.print_all_terms();

  return 0;
}
