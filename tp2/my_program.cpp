// Copyright 2013 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: jdtang@google.com (Jonathan Tang)
//
// Gets the cleantext of a page.

#include "gumbo.h"
#include "examples/clean_text.cc"
#include "examples/find_links.cc"
#include "examples/get_title.c"
#include "examples/positions_of_class.cc"

int main(int argc, char** argv) {
  // test_clean_text();
  test_find_links();
  // test_get_title();
  return 0;
}
