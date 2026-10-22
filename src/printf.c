#include "printf.h"

static size_t cursor_x = 0;
static size_t cursor_y = 0;

void init_printer(void) {
  vga_clear_screen();
  cursor_x = 0;
  cursor_y = 0;
}

void print_sym(char c) {
  if (c == '\n') {
    cursor_x = 0;
    cursor_y += 1;
  } else if (c == '\r') {
    cursor_x = 0;
  } else {
    vga_print_char(c, cursor_x, cursor_y);
    cursor_x += 1;
    if (cursor_x >= MAX_X) {
      cursor_x = 0;
      cursor_y += 1;
    }
  }

  if (cursor_y >= MAX_Y) {
    cursor_y = MAX_Y - 1;
    vga_scroll_down();
  }
}

static void print_string(const char* str) {
  if (!str) {
    str = "(null)";
  }
  while (*str) {
    print_sym(*str++);
  }
}

static void print_int(int val) {
  char buf[16];
  char* ptr = buf + sizeof(buf) - 1;
  *ptr = '\0';

  unsigned int uval;
  if (val < 0) {
    print_sym('-');
    uval = -(unsigned int)val;
  } else {
    uval = (unsigned int)val;
  }

  if (uval == 0) {
    print_sym('0');
    return;
  }

  while (uval > 0) {
    --ptr;
    *ptr = (char)('0' + (uval % 10));
    uval /= 10;
  }

  print_string(ptr);
}

static void print_hex(unsigned int val) {
  char buf[16];
  char* ptr = buf + sizeof(buf) - 1;
  *ptr = '\0';
  const char hex_nums[] = "0123456789abcdef";

  if (val == 0) {
    --ptr;
    *ptr = '0';
  } else {
    while (val > 0) {
      --ptr;
      *ptr = hex_nums[val % 16];
      val /= 16;
    }
  }

  --ptr;
  *ptr = 'x';
  --ptr;
  *ptr = '0';

  print_string(ptr);
}

void vprintf(const char* fmt, va_list args) {
  for (const char* cur = fmt; *cur != '\0'; ++cur) {
    if (*cur != '%') {
      print_sym(*cur);
      continue;
    }

    ++cur;
    if (*cur == '\0') {
      print_sym('%');
      break;
    }

    switch (*cur) {
      case 'd': {
        int val = va_arg(args, int);
        print_int(val);
        break;
      }
      case 'x': {
        unsigned int val = va_arg(args, unsigned int);
        print_hex(val);
        break;
      }
      case 'c': {
        char c = (char)va_arg(args, int);
        print_sym(c);
        break;
      }
      case 's': {
        const char* str = va_arg(args, const char*);
        print_string(str);
        break;
      }
      case '%': {
        print_sym('%');
        break;
      }
      default: {
        print_sym('%');
        print_sym(*cur);
        break;
      }
    }
  }
}

void printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}