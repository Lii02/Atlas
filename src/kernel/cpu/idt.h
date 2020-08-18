#ifndef IDT_H
#define IDT_H

#include "../std/stdlib.h"
#include "../std/stdint.h"

#define KERNEL_CS 0x8

struct __attribute__((packed)) idt_gate_t
{
    uint16_t low_offset;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t high_offset;
};

struct __attribute__((packed)) idt_register_t
{
    uint16_t limit;
    uint32_t base;
};

#define IDT_ENTRIES 256
struct idt_gate_t idt[IDT_ENTRIES];
struct idt_register_t idt_reg;

void set_idt_gate(int32_t n, uint32_t handler);
void set_idt();

#endif