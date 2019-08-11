# -------------------------------------------------------------------
# Beginning of Atlas bootloader makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
CFLAGS := -Wall -fno-builtin -fno-pie -nostdlib -ffreestanding -nostdinc -m32

# When adding new module asm files, add them as entries to the bottom of this file.
# Use the following template for assembly (.s) files.

# modules/FILENAME.o: modules/FILENAME.s
# 	$(AS) --32 -o modules/FILENAME.o modules/FILENAME.s

clear:
	$(RM) -f *.o
	$(RM) -f *.elf
	$(RM) -f *.bin

all: init.bin

init.bin: init.elf
	objcopy -O binary -j .text init.elf init.bin

init.elf: init.o
	$(LD) -o init.elf init.o -Ttext 0x7c00

init.o: init.s
	$(AS) -o init.o init.s
