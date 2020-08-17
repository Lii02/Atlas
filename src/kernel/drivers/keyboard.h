#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#define ESC_PRESSED 0x1
#define KEY1_PRESSED 0x2
#define KEY2_PRESSED 0x3
#define KEY3_PRESSED 0x4
#define KEY4_PRESSED 0x5
#define KEY5_PRESSED 0x6
#define KEY6_PRESSED 0x7
#define KEY7_PRESSED 0x8
#define KEY8_PRESSED 0x9
#define KEY9_PRESSED 0xA
#define KEY0_PRESSED 0xB
#define KEYSUB_PRESSED 0xC
#define KEYEQ_PRESSED 0xD
#define KEYBACKSPACE_PRESSED 0xE
#define KEYTAB_PRESSED 0xF
#define KEYQ_PRESSED 0x10
#define KEYW_PRESSED 0x11
#define KEYE_PRESSED 0x12
#define KEYR_PRESSED 0x13
#define KEYT_PRESSED 0x14
#define KEYY_PRESSED 0x15
#define KEYU_PRESSED 0x16
#define KEYI_PRESSED 0x17
#define KEYO_PRESSED 0x18
#define KEYP_PRESSED 0x19
#define KEYLBRKT_PRESSED 0x1A
#define KEYRBRKT_PRESSED 0x1B
#define KEYENTER_PRESSED 0x1C

#define NO_LOCK_FLAG (1 << 0)
#define CAPS_LOCK_FLAG (1 << 2)
#define SCROLL_LOCK_FLAG (1 << 2)
#define NUM_LOCK_FLAG (1 << 3)

#define MAX_KEYBOARD_INSTANCES 0x10

struct keyboard_t
{
    uint8_t current_key;
    uint8_t lock_flag;
    uint8_t id;
    void(*key_callback)(uint8_t, uint8_t);
};

struct keyboard_t* init_keyboard_driver(uint8_t id, void(*key_callback)(uint8_t, uint8_t));
void set_current_keyboard(uint8_t i);

#endif