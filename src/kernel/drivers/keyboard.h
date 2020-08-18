#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#define KEY_ESC_PRESSED 0x1
#define KEY_1_PRESSED 0x2
#define KEY_2_PRESSED 0x3
#define KEY_3_PRESSED 0x4
#define KEY_4_PRESSED 0x5
#define KEY_5_PRESSED 0x6
#define KEY_6_PRESSED 0x7
#define KEY_7_PRESSED 0x8
#define KEY_8_PRESSED 0x9
#define KEY_9_PRESSED 0xA
#define KEY_0_PRESSED 0xB
#define KEY_SUB_PRESSED 0xC
#define KEY_EQUAL_PRESSED 0xD
#define KEY_BACKSPACE_PRESSED 0xE
#define KEY_TAB_PRESSED 0xF
#define KEY_Q_PRESSED 0x10
#define KEY_W_PRESSED 0x11
#define KEY_E_PRESSED 0x12
#define KEY_R_PRESSED 0x13
#define KEY_T_PRESSED 0x14
#define KEY_Y_PRESSED 0x15
#define KEY_U_PRESSED 0x16
#define KEY_I_PRESSED 0x17
#define KEY_O_PRESSED 0x18
#define KEY_P_PRESSED 0x19
#define KEY_LBRKT_PRESSED 0x1A
#define KEY_RBRKT_PRESSED 0x1B
#define KEY_ENTER_PRESSED 0x1C
#define KEY_LCTRL_PRESSED 0x1D
#define KEY_A_PRESSED 0x1E
#define KEY_S_PRESSED 0x1F
#define KEY_D_PRESSED 0x20
#define KEY_F_PRESSED 0x21
#define KEY_G_PRESSED 0x22
#define KEY_H_PRESSED 0x23
#define KEY_J_PRESSED 0x24
#define KEY_K_PRESSED 0x25
#define KEY_L_PRESSED 0x26
#define KEY_SEMICOLON_PRESSED 0x27
#define KEY_QUOTE_PRESSED 0x28
#define KEY_BACKTICK_PRESSED 0x29
#define KEY_LSHIFT_PRESSED 0x2A
#define KEY_BACKSLASH_PRESSED 0x2B
#define KEY_Z_PRESSED 0x2C
#define KEY_X_PRESSED 0x2D
#define KEY_C_PRESSED 0x2E
#define KEY_V_PRESSED 0x2F
#define KEY_B_PRESSED 0x30
#define KEY_N_PRESSED 0x31
#define KEY_M_PRESSED 0x32
#define KEY_COMMA_PRESSED 0x33
#define KEY_PERIOD_PRESSED 0x34
#define KEY_FWDSLASH_PRESSED 0x35
#define KEY_RSHIFT_PRESSED 0x36
#define KEY_LALT_PRESSED 0x38
#define KEY_SPACE_PRESSED 0x39
#define KEY_CAPSLOCK_PRESSED 0x3A
#define KEY_NUMLOCK_PRESSED 0x45
#define KEY_SCROLLLOCK_PRESSED 0x46

#define NO_LOCK_FLAG (1 << 0)
#define CAPS_LOCK_FLAG (1 << 2)
#define SCROLL_LOCK_FLAG (1 << 2)
#define NUM_LOCK_FLAG (1 << 3)

#define MAX_KEYBOARD_INSTANCES 0x10 // 16 for now

struct keyboard_t
{
    uint8_t current_key;
    uint8_t lock_flag;
    uint8_t id;
    void(*key_callback)(uint8_t, uint8_t);
};

struct keyboard_t* init_keyboard_driver(uint8_t id, void(*key_callback)(uint8_t, uint8_t));
void shell_interpret_command();
void set_current_keyboard(uint8_t i);

#endif