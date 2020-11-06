#include <atlas/vga/vga.h>
#include <atlas/i386/i386asm.h>
#include <atlas/asm/irq.h>
#include <libc/stdio.h>
#include <atlas/ata.h>

#define WIDTH 80
#define HEIGHT 25

void kernel_main()
{
	// initialize the kernel
	isr_install();
	ata_initialize();
	
    init_vga(WIDTH, HEIGHT);
	init_stdout(WIDTH * HEIGHT, vga_putc);
    set_vga_colors(VGA_LIGHT_GREY, VGA_BLACK);
	clear_vga();

	//printf("part@atlas> ");
	vga_disable_cursor();

	free_vga();
	free_stdout();
}
