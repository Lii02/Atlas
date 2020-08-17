#include "asm.h"
#include "vga.h"
#include "mem.h"
#include "../cpu/isr.h"

extern uintptr_t current_break;

void kernel_main()
{
	isr_install();
	init_keyboard();

	init_terminal(80, 24);
	cl_terminal(VGA_LIGHT_GREY, VGA_BLACK);
	puts("Atlas v1.0 by Atlas Team\n\r");
	puts("Initialized 4K bytes of stack memory\n\r");
	puts("[root@atlas]: ");
}