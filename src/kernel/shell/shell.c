#include "shell.h"
#include <string.h>
#include "../core/vga.h"

struct shell_t create_shell()
{
    struct shell_t s;
    s.working_dir = "root/";
    s.min_cursor_pos = 0;
    return s;
}

void print_shell_prefix(struct shell_t* s)
{
    char* s1 = "$atlas[";
    puts(s1);
    s->min_cursor_pos += strlen(s1);
	puts(s->working_dir);
    s->min_cursor_pos += strlen(s->working_dir);
    char* s2 = "]: ";
	puts(s2);
    s->min_cursor_pos += strlen(s2);
}