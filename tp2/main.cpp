#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

// #include <algorithm>

// #include <sstream>
// #include <iterator>
// #include <iomanip>

#include "gumbo.h"
#include "utils.h"
#include "term.h"

void check_if_in_eof(std::ifstream &in) {
  if (in.eof()) {
    std::cout << FAIL << "Unexpected EOF" << ENDC << '\n';
    exit(EXIT_FAILURE);
  }
}

std::string read_data(std::ifstream &in, size_t &block_size) {
  char *buffer = new char[block_size];
  size_t position = in.tellg();

  in.seekg(position - block_size - 1);
  in.read(buffer, block_size);

  std::string response = std::string(buffer, block_size);

  delete[] buffer;

  return std::move(response);
}

static std::string cleantext(GumboNode *node) {
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {
    std::string contents = "";
    GumboVector *children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      const std::string text = cleantext((GumboNode *)children->data[i]);
      if (i != 0 && !text.empty()) {
        contents.append(" ");
      }
      contents.append(text);
    }
    return contents;
  } else {
    return "";
  }
}

std::vector<Term> create_vocabulary(std::vector<std::string> &strings) {
  std::vector<Term> vocabulary;
  bool not_found;
  for (const std::string &word : strings) {
    if (word.length() > 0) {
      size_t size = vocabulary.size();
      size_t count_add = 0;
      not_found = true;
      for (size_t i = 0; i < size + count_add; i++) {
        if (word.compare(vocabulary[i].get_word()) == 0) {
          vocabulary[i].plus_one();
          not_found = false;
          break;
        }
      }
      if (not_found) {
        vocabulary.push_back(Term(word));
        count_add++;
      }
    }
  }
  return vocabulary;
}

int create_inverted_index(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <html collections>\n";
    exit(EXIT_FAILURE);
  }

  const char *filename = argv[1];

  std::ifstream in(filename, std::ios::in | std::ios::binary);

  if (!in) {
    std::cout << "File " << filename << " not found!\n";
    exit(EXIT_FAILURE);
  }

  in.seekg(0, std::ios::end);
  size_t file_size = in.tellg();
  in.seekg(0, std::ios::beg);

  // check if have initial "|||"
  if (file_size > 3) {
    for (size_t i = 0; i < 3; i++) {
      if (in.get() != '|') {
        std::cout << "File " << filename << " with bad format!" << '\n';
        exit(EXIT_FAILURE);
      }
    }
  }

  size_t block_size, position;
  char c;
  std::string url, content;
  std::vector<std::string> words;

  int count = 0;
  while (!in.eof()) {
    std::cout << WARNING << "Read URL" << ENDC << '\n';
    // Read URL
    block_size = 0;
    while (in.get() != int('|')) {
      check_if_in_eof(in);
      block_size++;
    }

    // read URL
    url = read_data(in, block_size);
    std::cout << url << '\n';
    in.get();

    // Read block
    std::cout << WARNING << "Read HTML" << ENDC << '\n';
    block_size = 0;
    while (in.get() != int('|')) {
      check_if_in_eof(in);
      block_size++;
    }

    // read HTML
    content = read_data(in, block_size);
    content = iso_8859_1_to_utf8(content);

    GumboOutput *output = gumbo_parse(content.c_str());
    words = split_string(cleantext(output->root));
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    // TODO: make insert in inverted index
    create_vocabulary(words);

    // read "|||"
    for (size_t i = 0; i < 3; i++) {
      in.get(c);
      check_if_in_eof(in);
      if (c != '|') {
        std::cout << FAIL << "File " << filename << " with bad format!" << ENDC
                  << '\n';
        exit(EXIT_FAILURE);
      }
    }

    position = in.tellg();
    if (position >= file_size - 1)
      break;
    if (++count == 10) {
      break;
    }
  }

  in.close();

  return EXIT_SUCCESS;
}

// int main(int argc, char **argv) {
//   create_inverted_index(argc, argv);
//   return 0;
// }
