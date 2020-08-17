#include "idt.h"

#define LOW16(address) (uint16_t)((address) & 0xFFFF)
#define HIGH16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

void set_idt_gate(int32_t n, uint32_t handler)
{
    idt[n].low_offset = LOW16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = HIGH16(handler);
}

void set_idt()
{
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(struct idt_gate_t) - 1;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}