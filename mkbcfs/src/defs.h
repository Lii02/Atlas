
#ifndef DEFS_H
#define DEFS_H

typedef unsigned char      uint_8;
typedef unsigned short int uint_16;
typedef unsigned int       uint_32;
typedef unsigned long int  uint_64;

typedef char      int_8;
typedef short int int_16;
typedef int       int_32;
typedef long int  int_64;

#define BLOCK_SIZE 1024

#define MAJ_VER 1
#define MIN_VER 0
#define ALLOWED_WRITES 256
#define INODE_SIZE 64

typedef struct
{
   uint_8 data[BLOCK_SIZE];
} block_t;

#endif
