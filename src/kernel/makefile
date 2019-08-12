# -------------------------------------------------------------------
# Beginning of Atlas kernel makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
CFLAGS := -Wall -fno-builtin -fno-pie -nostdlib -ffreestanding -nostdinc -m32

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

clear:
	$(RM) -f *.o
	$(RM) -f *.elf
	$(RM) -f modules/*.o

all: ../../ext2/kernel.sys

../../ext2/kernel.sys: kernel.elf
	objcopy -O binary kernel.elf ../../ext2/kernel.sys

# IMPORTANT! Add all new module C/S files to this dependency list and to the compile list
# on the line below.

#                                          ||                                  ||
#                                          ||                                  ||
# Add dependencies here                    \/          ...and here...          \/
kernel.elf: kernel.o modules/kernel_init.o modules/atlas_std.o modules/vga.o
	$(CC) $(CFLAGS) -T link.ld -o kernel.elf kernel.o modules/kernel_init.o modules/atlas_std.o modules/vga.o

kernel.o: kernel.s
	$(AS) --32 -o kernel.o kernel.s

modules/kernel_init.o: modules/kernel_init.c
	$(CC) $(CFLAGS) -o modules/kernel_init.o -c modules/kernel_init.c

modules/atlas_std.o: modules/atlas_std.c
	$(CC) $(CFLAGS) -o modules/atlas_std.o -c modules/atlas_std.c

modules/vga.o: modules/vga.c
	$(CC) $(CFLAGS) -o modules/vga.o -c modules/vga.c
