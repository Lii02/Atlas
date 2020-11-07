#include <atlas/vga/vga.h>
#include <atlas/i386/i386asm.h>
#include <atlas/asm/irq.h>
#include <libc/stdio.h>
#include <atlas/ata.h>
#include <atlas/fs/bcfs.h>
#include <atlas/fs/vfs.h>
#include <libc/string.h>
#include <libc/malloc.h>

#define WIDTH 80
#define HEIGHT 25

// TODO(ldi): fix malloc bugs(if any)

void kernel_main()
{
	// initialize the kernel
	isr_install();
	ata_initialize();
	init_bcfs();

	init_vga(WIDTH, HEIGHT);
	init_stdout(WIDTH * HEIGHT, vga_putc);
	set_vga_colors(VGA_LIGHT_GREY, VGA_BLACK);
	clear_vga();
	vga_disable_cursor();
	
	puts("printf test\n");

	puts(primary_volume_m.ata_device->identity.model);

	free_vga();
	free_stdout();
	free_bcfs();
}
