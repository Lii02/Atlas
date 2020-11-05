#include <atlas/vga/vga.h>
#include <atlas/i386/i386asm.h>
#include <atlas/asm/irq.h>
#include <libc/stdio.h>

#define WIDTH 80
#define HEIGHT 25

void key_callback(uint8_t key, uint8_t flag)
{
	putchar(get_ascii(key));
}

void kernel_main()
{
	// initialize the kernel
	isr_install();

    init_vga(WIDTH, HEIGHT);
	init_stdout(WIDTH * HEIGHT, vga_putc);
    set_vga_colors(VGA_LIGHT_GREY, VGA_BLACK);
	clear_vga();
	init_keyboard(key_callback);
	printf("part@atlas> ");

	free_vga();
	free_stdout();
}
