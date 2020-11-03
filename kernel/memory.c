#include <atlas/memory.h>

void memset(void* data, int v, int length)
{
	unsigned char* temp_buff = (unsigned char*)data;
	for(; length != 0; length--)
	{
		(*temp_buff++) = v;
	}
	data = (void*)temp_buff;
}
