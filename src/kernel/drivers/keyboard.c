#include "keyboard.h"
#include <stdint.h>
#include "../core/asm.h"
#include "../cpu/isr.h"
#include "../core/vga.h"

static void shell_keyboard_callback(struct registers_t regs)
{
    uint8_t scancode = inb(0x60);
    print_letter(scancode);
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, shell_keyboard_callback);
}

void print_letter(uint8_t scancode)
{
    switch (scancode) {
        case 0x1:
            puts("ESC");
            break;
        case 0x2:
            puts("1");
            break;
        case 0x3:
            puts("2");
            break;
        case 0x4:
            puts("3");
            break;
        case 0x5:
            puts("4");
            break;
        case 0x6:
            puts("5");
            break;
        case 0x7:
            puts("6");
            break;
        case 0x8:
            puts("7");
            break;
        case 0x9:
            puts("8");
            break;
        case 0x0A:
            puts("9");
            break;
        case 0x0B:
            puts("0");
            break;
        case 0x0C:
            puts("-");
            break;
        case 0x0D:
            puts("+");
            break;
        case 0x0E:
            puts("\b");
            break;
        case 0x0F:
            puts("    ");
            break;
        case 0x10:
            puts("Q");
            break;
        case 0x11:
            puts("W");
            break;
        case 0x12:
            puts("E");
            break;
        case 0x13:
            puts("R");
            break;
        case 0x14:
            puts("T");
            break;
        case 0x15:
            puts("Y");
            break;
        case 0x16:
            puts("U");
            break;
        case 0x17:
            puts("I");
            break;
        case 0x18:
            puts("O");
            break;
        case 0x19:
            puts("P");
            break;
		case 0x1A:
			puts("[");
			break;
		case 0x1B:
			puts("]");
			break;
		case 0x1C:
			puts("\r");
			break;
		case 0x1D:
			//puts("LCtrl");
			break;
		case 0x1E:
			puts("A");
			break;
		case 0x1F:
			puts("S");
			break;
        case 0x20:
            puts("D");
            break;
        case 0x21:
            puts("F");
            break;
        case 0x22:
            puts("G");
            break;
        case 0x23:
            puts("H");
            break;
        case 0x24:
            puts("J");
            break;
        case 0x25:
            puts("K");
            break;
        case 0x26:
            puts("L");
            break;
        case 0x27:
            puts(";");
            break;
        case 0x28:
            puts("'");
            break;
        case 0x29:
            puts("`");
            break;
		case 0x2A:
			//puts("LShift");
			break;
		case 0x2B:
			puts("\\");
			break;
		case 0x2C:
			puts("Z");
			break;
		case 0x2D:
			puts("X");
			break;
		case 0x2E:
			puts("C");
			break;
		case 0x2F:
			puts("V");
			break;
        case 0x30:
            puts("B");
            break;
        case 0x31:
            puts("N");
            break;
        case 0x32:
            puts("M");
            break;
        case 0x33:
            puts(",");
            break;
        case 0x34:
            puts(".");
            break;
        case 0x35:
            puts("/");
            break;
        case 0x36:
            //puts("RShift");
            break;
        case 0x37:
            puts("*");
            break;
        case 0x38:
            //puts("LAlt");
            break;
        case 0x39:
            puts(" ");
            break;
    }
}