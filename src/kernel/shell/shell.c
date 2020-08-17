#include "shell.h"
#include <string.h>
#include "../core/vga.h"

extern vga_terminal terminal;
struct shell_t shells[MAX_SHELL_COUNT];
uint32_t current_shell;

struct shell_t* create_shell(uint32_t id, struct keyboard_t* kb)
{
    if(id > MAX_SHELL_COUNT)
    {
        puts("Shell instance out of bounds!");
    }
    shells[id].id = id;
    shells[id].kb = kb;
    shells[id].working_dir = "root/";
    shells[id].min_cursor_pos = 0;
    return &shells[id];
}

void print_shell_prefix(uint32_t shell_id)
{
    char* s1 = "$atlas[";
    puts(s1);
    shells[shell_id].min_cursor_pos += strlen(s1);
	puts(shells[shell_id].working_dir);
    shells[shell_id].min_cursor_pos += strlen(shells[shell_id].working_dir);
    char* s2 = "]: ";
	puts(s2);
    shells[shell_id].min_cursor_pos += strlen(s2);
}

void shell_key_callback(uint8_t key, uint8_t flag)
{
    switch(key)
    {
        case KEY1_PRESSED:
            putc('1');
            break;
        case KEY2_PRESSED:
            putc('2');
            break;
        case KEYBACKSPACE_PRESSED:
            break;
    }
}

void set_current_shell(uint32_t id)
{
    current_shell = 0;
}