#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp_at_pusha;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  uint32_t gs;
  uint32_t fs;
  uint32_t es;
  uint32_t ds;

  uint32_t vector;
  uint32_t error_code;
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
} context;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
  uint16_t limit;
  uint32_t base;
} idtr;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
  uint16_t offset1;
  uint16_t segment_selector;
  uint8_t reserved;
  uint8_t type;
  uint16_t offset2;
} interrupt_descriptor;
#pragma pack(pop)

_Static_assert(sizeof(context) == 68, "context size mismatch");

_Static_assert(sizeof(idtr) == 6, "idtr size mismatch");

_Static_assert(sizeof(interrupt_descriptor) == 8, "interrupt_descriptor size mismatch");

void idt_set_gate(uint8_t vector, uint32_t handler);

void init_interrupts();

void universal_handler(context* ctx);

extern void idt_load(idtr* desc);

extern void exp1();
