#include "vga_driver.h"
#include "memutils.h"

void vga_clear_screen() {
  short* buffer = (short*)VGA_ADDRESS;

  for (size_t y = 0; y < MAX_Y; ++y) {
    for (size_t x = 0; x < MAX_X; ++x) {
      buffer[x + y*MAX_X] = 0x0F<<8 | ' ';
    }
  }
}

void vga_print_char(char c, size_t x, size_t y) {
  short* buffer = (short*)VGA_ADDRESS;
  buffer[x + y * MAX_X] = 0x0F << 8 | c;
}

void vga_scroll_down() {
  short* buffer = (short*)VGA_ADDRESS;
  memmove(buffer, buffer + MAX_X, (MAX_Y-1)*MAX_X*2);
  for (size_t x = 0; x < MAX_X; ++x) {
    buffer[(MAX_Y - 1) * MAX_X + x] = (0x0F << 8) | ' ';
  }
}
