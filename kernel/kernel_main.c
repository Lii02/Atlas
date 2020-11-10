#include <atlas/vga.h>
#include <atlas/i386/i386asm.h>
#include <atlas/asm/irq.h>
#include <libc/stdio.h>
#include <atlas/ata.h>
#include <atlas/fs/bcfs.h>
#include <atlas/fs/vfs.h>
#include <libc/string.h>
#include <libc/malloc.h>

#include <atlaslib/tree.h>

#define WIDTH 80
#define HEIGHT 25

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

	tree_t* test_tree = create_tree();

	free_tree(test_tree);
	free_vga();
	free_stdout();
	free_bcfs();
}
