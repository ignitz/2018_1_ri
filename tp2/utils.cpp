#include "utils.h"

std::string iso_8859_1_to_utf8(std::string &str) {
  using namespace std;
  string strOut;
  for (string::iterator it = str.begin(); it != str.end(); ++it) {
    uint8_t ch = *it;
    if (ch < 0x80) {
      strOut.push_back(ch);
    } else {
      strOut.push_back(0xc0 | ch >> 6);
      strOut.push_back(0x80 | (ch & 0x3f));
    }
  }
  return strOut;
}

void remove_illegal_chars(std::string &text) {
  std::string::iterator it;

  std::string illegalChars = ILLEGAL_CHARS;
  for (it = text.begin(); it < text.end(); ++it) {
    bool found = illegalChars.find(*it) != std::string::npos;
    if (found) {
      *it = ' ';
    }
  }
}

// need boost library
std::vector<std::string> split_string(std::string &text, char split) {
  std::vector<std::string> results;
  boost::split(results, text, [&split](char c) { return c == split; });
  return std::move(results);
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

std::string get_cleanText(std::string html) {
  GumboOutput *output = gumbo_parse(html.c_str());
  html = cleantext(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  return std::move(html);
};

static void get_terms_in_node(GumboNode *node, std::vector<Phrase> &phrases) {
  if (node->type == GUMBO_NODE_TEXT) {
    Phrase phrase;
    std::string content = std::string(node->v.text.text);
    phrase.offset = node->v.text.start_pos.offset;
    remove_illegal_chars(content);
    phrase.length = content.length();
    phrase.phrase = content;
    phrases.push_back(std::move(phrase));
    return;
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {
    GumboVector *children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      get_terms_in_node((GumboNode *)children->data[i], phrases);
    }
    return;
  } else {
    return;
  }
}

// split_terms and save position from each term
std::vector<Term> split_terms(std::vector<Phrase> &phrases, size_t document_id) {
  std::vector<Term> terms;
  int state;
  std::string term;

  // state machine
  for (auto &phrase : phrases) {
    state = 0;
    term = "";
    for (size_t i = 0; i < phrase.length; i++) {
      switch (state) {
      case 0:
        if (phrase.phrase[i] != ' ') {
          term += phrase.phrase[i];
          state = 1;
        }
        break;

      case 1:
        if (phrase.phrase[i] == ' ' && term.length() != 0) {
          Term aux;
          std::transform(term.begin(), term.end(), term.begin(), ::tolower);
          aux.document_id = document_id;
          aux.position = (phrase.offset + i - term.length());
          aux.term = term;
          terms.push_back(aux);
          term = "";
          state = 0;
        } else {
          term += phrase.phrase[i];
        }
        break;

      default:
        break;
      }
    }

    if (state == 1) {
      Term aux;
      std::transform(term.begin(), term.end(), term.begin(), ::tolower);
      aux.document_id = document_id;
      aux.position = (phrase.offset + phrase.length - term.length());
      aux.term = term;
      terms.push_back(aux);
    }
  }

  return std::move(terms);
}

std::vector<Term> get_terms(std::string html, size_t document_id) {
  std::vector<Phrase> phrases;
  GumboOutput *output = gumbo_parse(html.c_str());
  get_terms_in_node(output->root, phrases);
  gumbo_destroy_output(&kGumboDefaultOptions, output);

  return std::move(split_terms(phrases, document_id));
}

void write_term(Term & term) {
  std::string term_file_name = "terms/" + term.term;
  std::fstream arq(term_file_name, std::ios::app);
  if (!arq.is_open()) {
    std::cout << FAIL << "Fail to open term " << term.term <<ENDC << '\n';
    exit(EXIT_FAILURE);
  }
  arq.write((char*) & term.document_id, sizeof(size_t));
  arq.write((char*) & term.position, sizeof(size_t));
  arq.close();
}
