#include "vga_driver.h"
#include <stdarg.h>

void init_printer();

void print_sym(char c);

void vprintf(const char* fmt, va_list args);

void printf(const char* fmt, ...);