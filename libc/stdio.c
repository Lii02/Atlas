#include <libc/stdio.h>
#include <libc/malloc.h>
#include <libc/string.h>
#include <libc/abort.h>
#include <libc/stdint.h>
#include <libc/stdarg.h>

char putchar(char c)
{
	stdoutf->putc(c);
	return c;
}

void puts(const char* str)
{
	for(int i = 0; i < strlen(str); i++)
	{
		putchar(str[i]);
	}
}

int printf(const char* format, ...)
{
}

void init_stdout(unsigned int size, void(*putc)(char))
{
	stdoutf = (FILE*)malloc(sizeof(FILE));
	stdoutf->size = size;
	stdoutf->flags = 0;
	stdoutf->putc = putc;
}

void set_stdout(FILE* f)
{
	stdoutf = f;
}

void free_stdout()
{
	free(stdoutf);
}
