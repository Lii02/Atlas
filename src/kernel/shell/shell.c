#include "shell.h"
#include "../std/string.h"
#include "../core/vga.h"
#include "../core/mem.h"

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
    shells[id].debug = false;
    shells[id].min_cursor_pos = 0;
    shell_reset_cmd();
    return &shells[id];
}

void print_shell_prefix(uint32_t shell_id)
{
    shells[shell_id].min_cursor_pos += terminal.cursor_x;
    char* s1 = "$atlas[";
    puts(s1);
    shells[shell_id].min_cursor_pos += strlen(s1);
	puts(shells[shell_id].working_dir);
    shells[shell_id].min_cursor_pos += strlen(shells[shell_id].working_dir);
    char* s2 = "]: ";
	puts(s2);
    shells[shell_id].min_cursor_pos += strlen(s2);
}

void shell_reset_cmd()
{
    size_t i;
    for(i = 0; i < MAX_COMMAND_LENGTH; i++)
    {
        shells[current_shell].cmd[i] = '\0';
    }
}

void start_new_shell_line()
{
    shells[current_shell].min_cursor_pos = 0;
    shell_reset_cmd();
    print_shell_prefix(current_shell);
}

void shell_key_callback(uint8_t key, uint8_t flag)
{
    switch(key)
    {
        case KEY_ENTER_PRESSED:
            puts("\n\r");
            bool i = shell_interpret_command();
            if(!i)
            {
                puts("Unknown command: \"");
                puts(shells[current_shell].cmd);
                puts("\"\n\r");
            }
            start_new_shell_line();
            break;
        case KEY_BACKSPACE_PRESSED:
            if(terminal.cursor_x > shells[current_shell].min_cursor_pos)
            {
                putc('\b');
            }
            break;
        default:
            push_char_to_input(shells[current_shell].cmd, get_ascii(key));
            break;
    }
}

void set_current_shell(uint32_t id)
{
    current_shell = id;
}

void push_char_to_input(char* str, char c)
{
    int len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
    putc(c);
}

bool shell_interpret_command()
{
    return false;
}

void set_working_dir(char* dir)
{
    shells[current_shell].working_dir = dir;
}