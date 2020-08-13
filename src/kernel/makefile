# -------------------------------------------------------------------
# Beginning of Atlas kernel makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
CFLAGS := -w -fno-builtin -fno-pie -nostdlib -ffreestanding -nostdinc -m32 -I../../std

# When adding new module C/S files, add them as entries to the bottom of this file.
# Use the following templates.

# For assembly (.s) files:
# modules/FILENAME.o: modules/FILENAME.s
# 	$(AS) --32 -o modules/FILENAME.o modules/FILENAME.s

# For C (.c) files:
# modules/FILENAME.o: modules/FILENAME.c
#	$(CC) $(CFLAGS) -o modules/FILENAME.o -c modules/FILENAME.c

# Header (.h) files contain only declarations and are included by C files and
# therefore don't need to be compiled directly.

# IMPORTANT! Add all new module C/S files to this dependency list and to the compile list
# on the kernel.elf line.

clear:
	$(RM) -f *.o
	$(RM) -f *.elf
	$(RM) -f modules/*.o
	$(RM) -f modules/core/*.o

all: ../../bcfs/kernel.sys

../../bcfs/kernel.sys: kernel.elf
	objcopy -O binary kernel.elf ../../bcfs/kernel.sys

kernel.elf: kernel.o modules/core/kernel_init.o modules/core/vga.o modules/core/asm.o modules/core/keyboard.o modules/core/alloc.o
	$(CC) $(CFLAGS) -T link.ld -o kernel.elf kernel.o modules/core/kernel_init.o modules/core/vga.o modules/core/asm.o modules/core/keyboard.o modules/core/alloc.o -Ttext 0x2000

kernel.o: kernel.s
	$(AS) --32 -o kernel.o kernel.s

# Core Atlas Kernel

modules/core/kernel_init.o: modules/core/kernel_init.c
	$(CC) $(CFLAGS) -o modules/core/kernel_init.o -c modules/core/kernel_init.c

modules/core/vga.o: modules/core/vga.c
	$(CC) $(CFLAGS) -o modules/core/vga.o -c modules/core/vga.c

modules/core/asm.o: modules/core/asm.c
	$(CC) $(CFLAGS) -o modules/core/asm.o -c modules/core/asm.c

modules/core/keyboard.o: modules/core/keyboard.c
	$(CC) $(CFLAGS) -o modules/core/keyboard.o -c modules/core/keyboard.c

/core/alloc.o: modules/core/alloc.c
	$(CC) $(CFLAGS) -o modules/core/alloc.o -c modules/core/alloc.c