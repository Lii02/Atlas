#include "keyboard.h"
#include "../std/stdint.h"
#include "../core/asm.h"
#include "../cpu/isr.h"

struct keyboard_t keyboard_instances[MAX_KEYBOARD_INSTANCES];
uint8_t curr_keyboard_inst;

static void keyboard_callback(struct registers_t regs)
{
    uint8_t scancode = inb(0x60);
    keyboard_instances[curr_keyboard_inst].current_key = scancode;
    keyboard_instances[curr_keyboard_inst].key_callback(scancode, 0);
}

struct keyboard_t* init_keyboard_driver(uint8_t id, void(*key_callback)(uint8_t, uint8_t))
{
    if(id > MAX_KEYBOARD_INSTANCES)
    {
        puts("Keyboard instance out of bounds!");
    }
    keyboard_instances[id].key_callback = key_callback;
    keyboard_instances[id].id = id;
    keyboard_instances[id].lock_flag = 0;
    keyboard_instances[id].lock_flag |= NO_LOCK_FLAG;
    register_interrupt_handler(IRQ1, keyboard_callback);
    return &keyboard_instances[id];
}

void set_current_keyboard(uint8_t i)
{
    curr_keyboard_inst = i;
}