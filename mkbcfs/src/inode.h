#include <string>
#include <vector>
#include "defs.h"

#ifndef INODE_H
#define INODE_H

typedef struct
{
   uint_16  link_type;
   uint_16  alloc_type;
   uint_32  size_lo;
   uint_32  size_hi;
   uint_32  create_POSIX;
   uint_32  access_POSIX;
   uint_32  modify_POSIX;
   uint_16  userID;
   uint_16  permissions;
   uint_32  block_ptr;
} inode_t;

std::vector<block_t*>& mount_filesystem(const std::string& mntdir);

#endif
