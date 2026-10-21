#include "printf.h"

static size_t cursor_x = 0;
static size_t cursor_y = 0;

void init_printer() {
  vga_clear_screen();
  cursor_x = 0;
  cursor_y = 0;
}

void print_sym(char c) {
  vga_print_char(c, cursor_x, cursor_y);
  cursor_x += 1;
  if (cursor_x > MAX_X) {
    cursor_x = 0;
    cursor_y += 1;
  }
  if (cursor_y > MAX_Y) {
    cursor_y=MAX_Y-1;
    vga_scroll_down();
  }
}

void vprintf(const char* fmt, va_list args) {

}

void printf(const char* fmt, ...);