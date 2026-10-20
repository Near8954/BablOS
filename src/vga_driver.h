#define VGA_ADDRESS 0xB8000
#define MAX_X 80
#define MAX_Y 25

#include <stddef.h>

void vga_clear_screen();

void vga_print_char(char c, size_t x, size_t y);

void vga_scroll_down();