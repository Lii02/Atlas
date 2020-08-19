#ifndef BCFS_H
#define BCFS_H

#include "../std/stdint.h"

struct inode_t
{
    uint16_t link_type;
    uint16_t alloc_type;
    uint32_t size_lo;
    uint32_t size_hi;
    uint32_t create_POSIX;
    uint32_t access_POSIX;
    uint32_t modify_POSIX;
    uint16_t userID;
    uint16_t permissions;
    uint32_t block_ptr;
};

#endif