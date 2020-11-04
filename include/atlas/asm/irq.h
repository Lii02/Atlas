#ifndef ATLAS_IRQ_H
#define ATLAS_IRQ_H

#include "../../libc/stdint.h"
#include "../i386/i386asm.h"

#define KERNEL_DATA_SEGMENT 0x8

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

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

typedef void (*isr_t)(cpuregisters_t);
void isr_install();
void irq_handler(cpuregisters_t r);
void isr_handler(cpuregisters_t r);
void initialize_interrupt(int i, isr_t handler);
void set_idt_gate(int32_t n, uint32_t handler);
void set_idt();

#endif
