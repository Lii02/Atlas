#ifndef ATLAS_LIBC_STDIO_H
#define ATLAS_LIBC_STDIO_H

#define EOF (-1)

struct file_t
{
	unsigned int size;
	int flags;
	void (*putc)(char);
};
typedef struct file_t FILE;

FILE* stdoutf;

char putchar(char c);
void puts(const char* str);
int printf(const char* format, ...);
void init_stdout(unsigned int size, void(*putc)(char));
void set_stdout(FILE* f);
void free_stdout();

#endif
