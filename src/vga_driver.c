#include "vga_driver.h"

void vga_clear_screen() {
  short* buffer = (short*)VGA_ADDRESS;

  for (size_t y = 0; y < MAX_Y; ++y) {
    for (size_t x = 0; x < MAX_X; ++x) {
      buffer[x + y*MAX_X] = 0x0F<<8 | ' ';
    }
  }
}