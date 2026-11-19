#include "universal_handler.h"
#include "allocator.h"
#include "kernel_panic.h"
interrupt_descriptor* IDT;
extern uint32_t isr_stub_table[256];

void idt_set_gate(uint8_t vector, uint32_t handler) {
  IDT[vector].offset1 = handler & 0xFFFF;
  IDT[vector].segment_selector = 0x08;
  IDT[vector].reserved = 0;
  IDT[vector].type= 0x8E; // present 1 dpl 00 fixed 0 D 1 Type 110
  IDT[vector].offset2 = (handler >> 16) & 0xFFFF;
}

void init_interrupts() {
  IDT = calloc_undead(256 * sizeof(interrupt_descriptor), 8);
  for (uint32_t i = 0; i < 256; ++i) {
    idt_set_gate(i, isr_stub_table[i]);
  }
  idtr descriptor;
  descriptor.base = (uint32_t)IDT;
  descriptor.limit = sizeof(interrupt_descriptor) * 256 - 1;
  idt_load(&descriptor);
}

void universal_handler(context* ctx) {
  kernel_panic(
      "Kernel panic: unhandled interrupt #%d at %x:%x\n\
  Registers:\n\
      EAX: %x, ECX: %x, EDX: %x, EBX: %x\n\
      ESP: %x, EBP: %x, ESI: %x, EDI: %x\n\
      DS : %x, ES : %x, FS : %x, GS : %x \n\
  Error code:\n\
      common_error_code, value: %x\n\
  EFLAGS:\n\
      value: %x",
      ctx->vector, ctx->cs, ctx->eip, ctx->eax, ctx->ecx, ctx->edx, ctx->ebx,
      ctx->esp_at_pusha, ctx->ebp, ctx->esi, ctx->edi, ctx->ds, ctx->es,
      ctx->fs, ctx->gs, ctx->error_code, ctx->eflags);
}