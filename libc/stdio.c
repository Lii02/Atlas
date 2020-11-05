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

static bool print(const char* data, size_t length)
{
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

static unsigned int get_digit_length (int n)
{
    if (n < 0) return get_digit_length ((n == INT32_MIN) ? INT32_MIN : -n);
	else if (n < 10) return 1;
    return 1 + get_digit_length(n / 10);
}

int printf(const char* format, ...)
{
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0')
	{
		size_t maxrem = INT32_MAX - written;
 
		if (format[0] != '%' || format[1] == '%')
		{
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c')
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		}
		else if(*format == 'd')
		{
			format++;
			int val = va_arg(parameters, int);
			size_t len = get_digit_length(val);
			char buff[len];
			itoa(val, buff, 10);
			if(!print(buff, len));
				return -1;
			written += buff;
		}
		else if (*format == 's')
		{
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (!print(str, len))
				return -1;
			written += len;
		}
		else
		{
			format = format_begun_at;
			size_t len = strlen(format);
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
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
