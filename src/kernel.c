#include "vga_driver.h"
void infloop();

void kernel_entry() {
  vga_clear_screen();
  infloop();
}