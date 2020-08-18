#include "asm.h"
#include "vga.h"
#include "mem.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../shell/shell.h"

void kernel_main()
{
	isr_install();
	init_terminal(80, 24);
	cl_terminal(VGA_LIGHT_GREY, VGA_BLACK);
	set_current_shell(0);

	struct keyboard_t* kb = init_keyboard_driver(0, shell_key_callback);
	set_current_keyboard(0);

	puts("Atlas v1.0 by Atlas Team\n\r");
	puts("Initialized 4K bytes of stack memory\n\r");

	struct shell_t* main_shell = create_shell(0, kb);

	start_new_shell_line();
}