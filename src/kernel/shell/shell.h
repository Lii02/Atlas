#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include "../drivers/keyboard.h"

struct shell_t
{
    char* working_dir;
    uint32_t min_cursor_pos;
    uint8_t current_key;
    struct keyboard_t* kb;
    char* submitted_command;
    uint32_t id;
};

#define MAX_SHELL_COUNT 0x10 // 16 for now

struct shell_t* create_shell(uint32_t id, struct keyboard_t* kb);
void print_shell_prefix(uint32_t shell_id);
void shell_key_callback(uint8_t key, uint8_t flag);
void set_current_shell(uint32_t id);

#endif