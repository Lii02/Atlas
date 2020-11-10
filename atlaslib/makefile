# -------------------------------------------------------------------
# Beginning of Atlas libc makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
AR := ar
KERNELHDRS := -I../include/
CFLAGS := -DI386=1 -w -fno-builtin -fno-pie -ffreestanding -nostdinc -m32 $(KERNELHDRS)

OBJS = \
	tree.o \

LIBS = -nostdlib
LINK_LIST = \
	$(LIBS) \
	$(OBJS) \

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
# on the libc.a line.

clean:
	$(RM) -f *.o
	$(RM) -f *.a

all: ../bcfs/atlaslib.al

../bcfs/atlaslib.al: atlaslib.a
	objcopy -O binary atlaslib.a ../bcfs/atlaslib.al
	
atlaslib.a: $(OBJS)
	$(LD) -m elf_i386 -o atlaslib.a $(LINK_LIST) ../libc/libc.a
	
tree.o: tree.c
	$(CC) $(CFLAGS) -o tree.o -c tree.c