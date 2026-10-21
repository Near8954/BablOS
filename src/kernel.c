#include "printf.h"
void infloop();

void kernel_entry() {
  init_printer();
  char c = 'a';
  while (1) {
    for (size_t i = 0; i < 26; ++i) {
      print_sym(c+i);
    }
  }
  infloop();
}