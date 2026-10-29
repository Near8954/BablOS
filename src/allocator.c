#include "allocator.h"
#include "memutils.h"

#define ARENA_START ((char*)0x100000)
#define ARENA_SIZE 0x100000
#define ARENA_END (ARENA_START + ARENA_SIZE)

static char* cur = ARENA_START;

static inline void* align_up(void* ptr, size_t align) {
  unsigned int addr = (unsigned int)ptr;
  unsigned int aligned_addr = (addr + (align - 1)) & ~(align - 1);
  return (char*)aligned_addr;
}

void* malloc_undead(size_t size, size_t alignment) {
  char* ptr = align_up(cur, alignment);
  if (ptr + size > ARENA_END) {
    return NULL;
  }
  cur = ptr + size;
  return (void*)ptr;
}

void* calloc_undead(size_t size, size_t alignment) {
  void* ptr = malloc_undead(size, alignment);
  if (ptr == NULL) {
    return NULL;
  }
  memzero(ptr, size);
  return ptr;
}