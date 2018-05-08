#include "term_manage.h"

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

void test_gumbo(std::string html, size_t document_id) {
  std::vector<Phrase> phrases;
  std::vector<Term> terms;
  GumboOutput *output = gumbo_parse(html.c_str());
  get_terms_in_node(output->root, phrases);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  // split_terms and save position

  int state;
  std::string term;
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
          Term aux = {
              document_id,                         // document_id
              (phrase.offset + i - term.length()), // position
              term,                                // term
          };
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
      Term aux = {
          document_id,                                     // document_id
          (phrase.offset + phrase.length - term.length()), // position
          term,                                            // term
      };
      terms.push_back(aux);
    }
  }
}
