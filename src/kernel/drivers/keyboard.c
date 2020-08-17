#include "keyboard.h"
#include <stdint.h>
#include "../core/asm.h"
#include "../cpu/isr.h"
#include "../core/vga.h"

struct keyboard_t keyboard_instances[MAX_KEYBOARD_INSTANCES];
uint8_t current_keyboard_instance;

static void keyboard_callback(struct registers_t regs)
{
    uint8_t scancode = inb(0x60);
    keyboard_instances[current_keyboard_instance].current_key = scancode;
}

struct keyboard_t init_keyboard_driver(uint8_t id)
{
    if(id > 0xFF)
    {
        puts("Keyboard instance out of bounds!");
    }
    keyboard_instances[id].id = id;
    keyboard_instances[id].used = true;
    register_interrupt_handler(IRQ1, keyboard_callback);
    return keyboard_instances[id];
}