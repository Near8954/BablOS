#include "printf.h"
#include "allocator.h"
#include "kernel_panic.h"
void infloop();

void test() {
  const size_t sizes[] = {13, 128, 47, 512, 1024, 7};
  const size_t aligns[] = {4, 16, 8, 64, 32, 16};
  const size_t test_cases_count = sizeof(sizes) / sizeof(sizes[0]);
  size_t i = 0;
  while (1) {
    size_t sz = sizes[i % test_cases_count];
    size_t al = aligns[i % test_cases_count];
    void* ptr = malloc_undead(sz, al);
    if (ptr == NULL) {
      kernel_panic("Out of memory in undead arena! Req: %d bytes, align %d\n", sz, al);
    }
    printf("Allocated %d bytes (align %d) at %x\n", sz, al, (unsigned int)ptr);
    ++i;;
  }
}

void kernel_entry() {
  test();
  infloop();
}