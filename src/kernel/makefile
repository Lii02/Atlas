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
# modules/FILENAME.o: modules/FILENAME.s
# 	$(AS) --32 -o modules/FILENAME.o modules/FILENAME.s

# For C (.c) files:
# modules/FILENAME.o: modules/FILENAME.c
#	$(CC) $(CFLAGS) -o modules/FILENAME.o -c modules/FILENAME.c

# Header (.h) files contain only declarations and are included by C files and
# therefore don't need to be compiled directly.

# IMPORTANT! Add all new module C/S files to this dependency list and to the compile list
# on the line below.

#                                          ||                                  ||
#                                          ||                                  ||
# Add dependencies here                    \/          ...and here...          \/

clear:
	$(RM) -f *.o
	$(RM) -f *.elf
	$(RM) -f modules/*.o

all: ../../bcfs/kernel.sys

# Core Atlas kernel

../../bcfs/kernel.sys: kernel.elf
	objcopy -O binary kernel.elf ../../bcfs/kernel.sys

kernel.elf: kernel.o modules/core/kernel.o modules/core/atlas_string.o modules/core/vga.o
	$(CC) $(CFLAGS) -T link.ld -o kernel.elf kernel.o modules/core/kernel.o modules/core/atlas_string.o modules/core/vga.o

kernel.o: kernel.s
	$(AS) --32 -o kernel.o kernel.s

modules/core/kernel.o: modules/core/kernel.c
	$(CC) $(CFLAGS) -o modules/core/kernel.o -c modules/core/kernel.c

modules/core/atlas_string.o: modules/core/atlas_string.c
	$(CC) $(CFLAGS) -o modules/core/atlas_string.o -c modules/core/atlas_string.c

modules/core/vga.o: modules/core/vga.c
	$(CC) $(CFLAGS) -o modules/core/vga.o -c modules/core/vga.c

# Atlas IO

modules/io/atlas_io.o: modules/io/atlas_io.c
	$(CC) $(CFLAGS) -o modules/io/atlas_io.o -c modules/io/atlas_io.c
