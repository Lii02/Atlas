#include "defs.h"
#include "vga.h"

/*
 * Atlas Kernel Main
 * 	- Atlas Team 2020
 */

void _kernel_init()
{
	init_terminal(80, 24);
	cl_terminal(VGA_LIGHT_GREY, VGA_BLACK);
	putsln("Atlas OS v1.0\n\rThe Atlas Team 2020\n\r");
	puts("root@atlas>");
}

void kernel_clean_up()
{   
}