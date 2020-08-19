# -------------------------------------------------------------------
# Beginning of Atlas kernel makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
CFLAGS := -w -fno-builtin -fno-pie -nostdlib -ffreestanding -nostdinc -m32

# When adding new module C/S files, add them as entries to the bottom of this file.
# Use the following templates.

# For assembly (.s) files:
# FILENAME.o: FILENAME.s
# 	$(AS) --32 -o FILENAME.o FILENAME.s

# For C (.c) files:
# FILENAME.o: FILENAME.c
#	$(CC) $(CFLAGS) -o FILENAME.o -c FILENAME.c

# Header (.h) files contain only declarations and are included by C files and
# therefore don't need to be compiled directly.

# IMPORTANT! Add all new module C/S files to this dependency list and to the compile list
# on the kernel.elf line.

clear:
	$(RM) -f *.o
	$(RM) -f *.elf
	$(RM) -f *.o
	$(RM) -f core/*.o
	$(RM) -f cpu/*.o
	$(RM) -f drivers/*.o
	$(RM) -f shell/*.o
	$(RM) -f std/*.o
	$(RM) -f fs/*.o

all: ../../bcfs/kernel.sys

../../bcfs/kernel.sys: kernel.elf
	objcopy -O binary kernel.elf ../../bcfs/kernel.sys

kernel.elf: kernel.o std/stdlib.o std/string.o core/kernel_init.o core/vga.o core/asm.o core/mem.o cpu/interrupts.o cpu/idt.o cpu/isr.o cpu/timer.o drivers/keyboard.o shell/shell.o fs/bcfs.o
	$(CC) $(CFLAGS) -T link.ld -o kernel.elf kernel.o std/stdlib.o std/string.o core/kernel_init.o core/vga.o core/asm.o core/mem.o cpu/interrupts.o cpu/idt.o cpu/isr.o cpu/timer.o drivers/keyboard.o shell/shell.o fs/bcfs.o -Ttext 0x2000

kernel.o: kernel.s
	$(AS) --32 -o kernel.o kernel.s

# Atlas STD
std/stdlib.o: std/stdlib.c
	$(CC) $(CFLAGS) -o std/stdlib.o -c std/stdlib.c
	
std/string.o: std/string.c
	$(CC) $(CFLAGS) -o std/string.o -c std/string.c

# Core Atlas Kernel

core/kernel_init.o: core/kernel_init.c
	$(CC) $(CFLAGS) -o core/kernel_init.o -c core/kernel_init.c

core/vga.o: core/vga.c
	$(CC) $(CFLAGS) -o core/vga.o -c core/vga.c

core/asm.o: core/asm.c
	$(CC) $(CFLAGS) -o core/asm.o -c core/asm.c

/core/alloc.o: core/mem.c
	$(CC) $(CFLAGS) -o core/mem.o -c core/mem.c

# Atlas CPU classes

cpu/interrupts.o: cpu/interrupts.s
	$(AS) --32 -o cpu/interrupts.o cpu/interrupts.s

cpu/isr.o: cpu/isr.c
	$(CC) $(CFLAGS) -o cpu/isr.o -c cpu/isr.c

cpu/idt.o: cpu/idt.c
	$(CC) $(CFLAGS) -o cpu/idt.o -c cpu/idt.c

cpu/timer.o: cpu/timer.c
	$(CC) $(CFLAGS) -o cpu/timer.o -c cpu/timer.c

# Atlas driver classes

drivers/keyboard.o: drivers/keyboard.c
	$(CC) $(CFLAGS) -o drivers/keyboard.o -c drivers/keyboard.c

# Atlas shell classes

shell/shell.o: shell/shell.c
	$(CC) $(CFLAGS) -o shell/shell.o -c shell/shell.c

# Atlas FS clases

fs/bcfs.o: fs/bcfs.c
	$(CC) $(CFLAGS) -o fs/bcfs.o -c fs/bcfs.c