#include "defs.h"

#ifndef ATLAS_STRING_H
#define ATLAS_STRING_H

typedef char* string;

uint_32 strlen(string str);
string strreplace(string* str, char c0, char c1);

#endif
