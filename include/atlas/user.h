#ifndef ATLAS_USER_H
#define ATLAS_USER_H
#include "../libc/stdint.h"

#define ADMIN_CONTROL (1 << 0)
#define BASIC_CONTROL (1 << 1)

struct user_t
{
	char* name;
	int8_t id;
	int8_t flag;
};

#endif
