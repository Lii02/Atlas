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
    // Caps lock acts as a shift key for special characters until I get shift keys working
    switch(key)
    {
        case KEY_1_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '1');
            break;
        case KEY_2_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '2');
            break;
        case KEY_3_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '3');
            break;
        case KEY_4_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '4');
            break;
        case KEY_5_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '5');
            break;
        case KEY_6_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '6');
            break;
        case KEY_7_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '7');
            break;
        case KEY_8_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '8');
            break;
        case KEY_9_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '9');
            break;
        case KEY_0_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '0');
            break;
        case KEY_TAB_PRESSED:
            push_char_to_input(shells[current_shell].cmd, '\t');
            break;
        case KEY_Q_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'Q');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'q');
            }
            break;
        case KEY_W_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'W');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'w');
            }
            break;
        case KEY_E_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'E');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'e');
            }
            break;
        case KEY_R_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'R');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'r');
            }
            break;
        case KEY_T_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'T');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 't');
            }
            break;
        case KEY_Y_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'Y');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'y');
            }
            break;
        case KEY_U_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'U');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'u');
            }
            break;
        case KEY_I_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'I');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'i');
            }
            break;
        case KEY_O_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'O');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'o');
            }
            break;
        case KEY_P_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'P');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'p');
            }
            break;
        case KEY_LBRKT_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '{');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, '[');
            }
            break;
        case KEY_RBRKT_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '}');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, ']');
            }
            break;
        case KEY_BACKSLASH_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '|');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, '\\');
            }
            break;
        case KEY_A_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'A');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'a');
            }
            break;
        case KEY_S_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'S');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 's');
            }
            break;
        case KEY_D_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'D');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'd');
            }
            break;
        case KEY_F_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'F');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'f');
            }
            break;
        case KEY_G_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'G');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'g');
            }
            break;
        case KEY_H_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'H');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'h');
            }
            break;
        case KEY_J_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'J');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'j');
            }
            break;
        case KEY_K_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'K');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'k');
            }
            break;
        case KEY_L_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'L');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'l');
            }
            break;
        case KEY_SEMICOLON_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, ':');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, ';');
            }
            break;
        case KEY_QUOTE_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '\"');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, '\'');
            }
            break;
        case KEY_Z_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'Z');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'z');
            }
            break;
        case KEY_X_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'X');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'x');
            }
            break;
        case KEY_C_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'C');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'c');
            }
            break;
        case KEY_V_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'V');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'v');
            }
            break;
        case KEY_B_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'B');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'b');
            }
            break;
        case KEY_N_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'N');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'n');
            }
            break;
        case KEY_M_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, 'M');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, 'm');
            }
            break;
        case KEY_COMMA_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '<');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, ',');
            }
            break;
        case KEY_PERIOD_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '>');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, '.');
            }
            break;
        case KEY_FWDSLASH_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '?');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, '/');
            }
            break;
        case KEY_SPACE_PRESSED:
            push_char_to_input(shells[current_shell].cmd, ' ');
            break;
        case KEY_SUB_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '_');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, '-');
            }
            break;
        case KEY_EQUAL_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                push_char_to_input(shells[current_shell].cmd, '+');
            }
            else
            {
                push_char_to_input(shells[current_shell].cmd, '=');
            }
            break;
        case KEY_CAPSLOCK_PRESSED:
            if(shells[current_shell].kb->lock_flag & CAPS_LOCK_FLAG)
            {
                shells[current_shell].kb->lock_flag &= ~CAPS_LOCK_FLAG;
            }
            else
            {
                shells[current_shell].kb->lock_flag |= CAPS_LOCK_FLAG;
            }
            break;
        case KEY_BACKSPACE_PRESSED:
            if(terminal.cursor_x > shells[current_shell].min_cursor_pos)
            {
                push_char_to_input(shells[current_shell].cmd, '\b');
            }
            break;
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
    if(strcmp(shells[current_shell].cmd, "clear") == 0)
    {
        vga_scroll(terminal.cursor_y);
        return true;
    }
    else if(strcmp(shells[current_shell].cmd, "debug") == 0)
    {
        shells[current_shell].debug = !shells[current_shell].debug;
        return true;
    }

    return false;
}

void set_working_dir(char* dir)
{
    shells[current_shell].working_dir = dir;
}