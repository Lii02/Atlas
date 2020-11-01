#include "keyboard.h"
#include "../std/stdint.h"
#include "../core/asm.h"
#include "../cpu/isr.h"

struct keyboard_t keyboard_instances[MAX_KEYBOARD_INSTANCES];
uint8_t current_kb;

static void keyboard_callback(struct registers_t regs)
{
    uint8_t scancode = inb(0x60);
    keyboard_instances[current_kb].current_key = scancode;
    keyboard_instances[current_kb].key_callback(scancode, 0);
}

struct keyboard_t* init_keyboard_driver(uint8_t id, void(*key_callback)(uint8_t, uint8_t))
{
    if(id > MAX_KEYBOARD_INSTANCES)
    {
        puts("Keyboard instance out of bounds!");
    }
    keyboard_instances[id].key_callback = key_callback;
    keyboard_instances[id].id = id;
    keyboard_instances[id].lock_flag |= NO_LOCK_FLAG;
    register_interrupt_handler(IRQ1, keyboard_callback);
    return &keyboard_instances[id];
}

void set_current_keyboard(uint8_t i)
{
    current_kb = i;
}

#define LEFT_OR_RIGHT keyboard_instances[current_kb].lock_flag & LSHIFT_LOCK_FLAG || keyboard_instances[current_kb].lock_flag & RSHIFT_LOCK_FLAG
#define CAPS_ON keyboard_instances[current_kb].lock_flag & CAPS_LOCK_FLAG

#define PUSH_CHAR_SHIFT_ONLY(upper, lower) \
            if(LEFT_OR_RIGHT) \
            { \
                return upper; \
            } \
            else \
            { \
                return lower; \
            } \

#define PUSH_CHAR_SHIFT_CAPS(upper, lower) \
            if(!(LEFT_OR_RIGHT && CAPS_ON) && (LEFT_OR_RIGHT || CAPS_ON)) \
            { \
                return upper; \
            } \
            else \
            { \
                return lower; \
            } \

char get_ascii(uint8_t byte)
{
    switch(byte)
    {
        case KEY_BACKTICK_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('~', '`')
            break;
        case KEY_1_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('!', '1')
            break;
        case KEY_2_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('@', '2')
            break;
        case KEY_3_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('#', '3')
            break;
        case KEY_4_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('$', '4')
            break;
        case KEY_5_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('%', '5')
            break;
        case KEY_6_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('^', '6')
            break;
        case KEY_7_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('^', '6')
            break;
        case KEY_8_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('*', '8')
            break;
        case KEY_9_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('(', '9')
            break;
        case KEY_0_PRESSED:
            PUSH_CHAR_SHIFT_ONLY(')', '0')
            break;
        case KEY_SUB_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('_', '-')
            break;
        case KEY_EQUAL_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('+', '=')
            break;
        case KEY_TAB_PRESSED:
            return '\t';
            break;
        case KEY_Q_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('Q', 'q')
            break;
        case KEY_W_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('W', 'w')
            break;
        case KEY_E_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('E', 'e')
            break;
        case KEY_R_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('R', 'r')
            break;
        case KEY_T_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('T', 't')
            break;
        case KEY_Y_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('Y', 'y')
            break;
        case KEY_U_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('U', 'u')
            break;
        case KEY_I_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('I', 'i')
            break;
        case KEY_O_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('O', 'o')
            break;
        case KEY_P_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('P', 'p')
            break;
        case KEY_LBRKT_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('{', '[')
            break;
        case KEY_RBRKT_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('}', ']')
            break;
        case KEY_BACKSLASH_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('|', '\\')
            break;
        case KEY_A_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('A', 'a')
            break;
        case KEY_S_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('S', 's')
            break;
        case KEY_D_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('D', 'd')
            break;
        case KEY_F_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('F', 'f')
            break;
        case KEY_G_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('G', 'g')
            break;
        case KEY_H_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('H', 'h')
            break;
        case KEY_J_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('J', 'j')
            break;
        case KEY_K_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('K', 'k')
            break;
        case KEY_L_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('L', 'l')
            break;
        case KEY_SEMICOLON_PRESSED:
            PUSH_CHAR_SHIFT_ONLY(':', ';')
            break;
        case KEY_QUOTE_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('\"', '\'')
            break;
        case KEY_Z_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('Z', 'z')
            break;
        case KEY_X_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('X', 'x')
            break;
        case KEY_C_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('C', 'c')
            break;
        case KEY_V_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('V', 'v')
            break;
        case KEY_B_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('B', 'b')
            break;
        case KEY_N_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('N', 'n')
            break;
        case KEY_M_PRESSED:
            PUSH_CHAR_SHIFT_CAPS('M', 'm')
            break;
        case KEY_COMMA_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('<', ',')
            break;
        case KEY_PERIOD_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('>', '.')
            break;
        case KEY_FWDSLASH_PRESSED:
            PUSH_CHAR_SHIFT_ONLY('?', '/')
            break;
        case KEY_LSHIFT_PRESSED:
            keyboard_instances[current_kb].lock_flag |= LSHIFT_LOCK_FLAG;
            break;
        case KEY_LSHIFT_RELEASED:
            keyboard_instances[current_kb].lock_flag &= ~LSHIFT_LOCK_FLAG;
            break;
        case KEY_RSHIFT_PRESSED:
            keyboard_instances[current_kb].lock_flag |= RSHIFT_LOCK_FLAG;
            break;
        case KEY_RSHIFT_RELEASED:
            keyboard_instances[current_kb].lock_flag &= ~RSHIFT_LOCK_FLAG;
            break;
        case KEY_CAPSLOCK_PRESSED:
            if(keyboard_instances[current_kb].lock_flag & CAPS_LOCK_FLAG)
            {
                keyboard_instances[current_kb].lock_flag &= ~CAPS_LOCK_FLAG;
            }
            else
            {
                keyboard_instances[current_kb].lock_flag |= CAPS_LOCK_FLAG;
            }
            break;
        case KEY_BACKSPACE_PRESSED:
            return '\b';
            break;
    }
    return 0x0;
}