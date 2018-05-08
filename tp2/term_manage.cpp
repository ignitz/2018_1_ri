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

static void walk_to_GumboNode(GumboNode *node) {
  if (node->type == GUMBO_NODE_TEXT) {
    std::string content = std::string(node->v.text.text);

    std::cout << BLUE << node->v.text.start_pos.offset << '\t'
              << content.length() << ENDC << '\n';
    remove_illegal_chars(content);
    std::cout << content << '\n';
    return;
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {
    GumboVector *children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      walk_to_GumboNode((GumboNode *)children->data[i]);
    }
    return;
  } else {
    return;
  }
}

std::string get_cleanText(std::string html) {
  html = iso_8859_1_to_utf8(html);
  GumboOutput *output = gumbo_parse(html.c_str());
  html = cleantext(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  return std::move(html);
};

void test_gumbo(std::string html) {
  GumboOutput *output = gumbo_parse(html.c_str());
  walk_to_GumboNode(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}
