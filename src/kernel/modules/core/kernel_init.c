#include "defs.h"
#include "vga.h"

/*
 * Atlas Kernel Main
 * 	- Atlas Team 2020
 */

void _kernel_init()
{
    init_terminal(80, 24);
	vga_write('A', 0, 0, VGA_LIGHT_GREY, VGA_BLACK);
    //puts("Atlas OS: by Atlas Team", 23);
}

void kernel_clean_up()
{
    
}