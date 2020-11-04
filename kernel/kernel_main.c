#include <atlas/vga/vga.h>
#include <atlas/i386/i386asm.h>
#include <atlas/asm/irq.h>
#include <libc/stdio.h>

void keyboard_interrupt(cpuregisters_t r)
{
	char key = CPUINB(0x60);
	putchar(key);
}

#define WIDTH 80
#define HEIGHT 25

void kernel_main()
{
	// initialize the kernel
	isr_install();
	initialize_interrupt(IRQ1, keyboard_interrupt);

    init_vga(WIDTH, HEIGHT);
	init_stdout(WIDTH * HEIGHT, vga_putc);
    set_vga_colors(VGA_LIGHT_GREY, VGA_BLACK);
	clear_vga();
	puts("part@atlas> ");

	free_vga();
	free_stdout();
}
