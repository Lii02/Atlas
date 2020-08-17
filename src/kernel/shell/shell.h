#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

struct shell_t
{
    char* working_dir;
    uint32_t min_cursor_pos;
    uint8_t current_key;
};

struct shell_t create_shell();
void print_shell_prefix(struct shell_t* s);

#endif