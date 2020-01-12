#include "defs.h"
#include "vga.h"

/*
 * Atlas Kernel Main
 * 	- Atlas Team 2020
 */

void _kernel_init()
{
    init_terminal(80, 24);
	cl_terminal(VGA_BLACK);
    puts("Atlas OS: by Atlas Team");
}

void kernel_clean_up()
{
    
}