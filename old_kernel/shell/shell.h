#ifndef SHELL_H
#define SHELL_H

#include "../std/stdint.h"
#include "../std/stdlib.h"
#include "../std/stdbool.h"
#include "../drivers/keyboard.h"

#define MAX_COMMAND_LENGTH 256

struct shell_t
{
    char* working_dir;
    uint32_t min_cursor_pos;
    uint8_t current_key;
    uint32_t id;
    bool debug;
    char cmd[MAX_COMMAND_LENGTH];
};

#define MAX_SHELL_COUNT 0x10 // 16 for now

struct shell_t* create_shell(uint32_t id);
void print_shell_prefix(uint32_t shell_id);
void shell_reset_cmd();
void start_new_shell_line();
void shell_key_callback(uint8_t key, uint8_t flag);
void set_current_shell(uint32_t id);
void push_char_to_input(char* str, char c);
bool shell_interpret_command();
void set_working_dir(char* dir);

#endif