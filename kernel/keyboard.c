#include <atlas/keyboard.h>
#include <atlas/i386/i386asm.h>
#include <atlas/asm/irq.h>

static struct keyboard_t current_kb;

static void keyboard_callback(cpuregisters_t regs)
{
    uint8_t scancode = CPUINB(0x60);
    current_kb.key = scancode;
    current_kb.key_callback(scancode, 0);
}

void init_keyboard(void(*key_callback)(uint8_t, uint8_t))
{
	current_kb.flag = 0;
	current_kb.key_callback = key_callback;
	current_kb.flag |= NO_LOCK_FLAG;
    initialize_interrupt(IRQ1, keyboard_callback);
}

#define LEFT_OR_RIGHT current_kb.flag & LSHIFT_LOCK_FLAG || current_kb.flag & RSHIFT_LOCK_FLAG
#define CAPS_ON current_kb.flag & CAPS_LOCK_FLAG

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
		case KEY_SPACE_PRESSED:
			return ' ';
			break;
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
           	current_kb.flag |= LSHIFT_LOCK_FLAG;
            break;
        case KEY_LSHIFT_RELEASED:
            current_kb.flag &= ~LSHIFT_LOCK_FLAG;
            break;
        case KEY_RSHIFT_PRESSED:
            current_kb.flag |= RSHIFT_LOCK_FLAG;
            break;
        case KEY_RSHIFT_RELEASED:
            current_kb.flag &= ~RSHIFT_LOCK_FLAG;
            break;
        case KEY_CAPSLOCK_PRESSED:
            if(current_kb.flag & CAPS_LOCK_FLAG)
            {
                current_kb.flag &= ~CAPS_LOCK_FLAG;
            }
            else
            {
                current_kb.flag |= CAPS_LOCK_FLAG;
			}
            break;
        case KEY_BACKSPACE_PRESSED:
            return '\b';
            break;
    }
    return 0x0;
}
