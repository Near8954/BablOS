#include "kernel_panic.h"

void vkernel_panic(const char* fmt, va_list args) {
  printf("\n--- KERNEL PANIC ---\n");
  vprintf(fmt, args);
  cli();
  infloop();
}

void kernel_panic(const char* fmt, ...) {
  init_printer();
  va_list args;
  va_start(args, fmt);
  vkernel_panic(fmt, args);
  va_end(args);
}