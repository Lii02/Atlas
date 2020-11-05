#include <libc/abort.h>
#include <libc/stdio.h>

void abort()
{
	puts("atlas: panic()\n");
}
