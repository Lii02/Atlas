/*
 * Atlas string
*/

#include "atlas_layer.h"

#ifndef ATLAS_STRING_H
#define ATLAS_STRING_H

typedef char* atlas_string;

unsigned int strlen(atlas_string str);
atlas_string strreplace(atlas_string *str, char rep, char with);

#endif