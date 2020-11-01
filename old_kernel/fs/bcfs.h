#ifndef BCFS_H
#define BCFS_H

#include "../std/stdint.h"

// BCFS state
#define FS_CLEAN 0x0
#define FS_ERRORS 0x1

// Error compensation
#define IGNORE_ERRORS 0x0
#define MOUNT_DISK_READ_ONLY 0x1
#define KERNEL_PANIC 0x2

// Inode link type
#define UNALLOCATED_INODE 0x0
#define DATA_PTR 0x1
#define INODE_TABLE_PTR 0x2

// Allocated type
#define STANDARD_FILE 0x0
#define DIRECTORY 0x1

// Inode permissions
#define USER_READ 0b0000000000000001
#define USER_WRITE 0b0000000000000010
#define USER_EXECUTE 0b0000000000000100
#define USER_MANAGE 0b0000000000001000
#define GROUP_READ 0b0000000000010000
#define GROUP_WRITE 0b0000000000100000
#define GROUP_EXECUTE 0b0000000001000000
#define GROUP_MANAGE 0b0000000010000000
#define OTHER_READ 0b0000000100000000
#define OTHER_WRITE 0b0000001000000000
#define OTHER_EXECUTE 0b0000010000000000
#define OTHER_MANAGE 0b0000100000000000

#define VOLUME_NAME_LENGTH 16
#define MOUNT_DIR_LENGTH 64

struct volume_desc_t
{
   uint16_t  BCFS_signature;
   uint32_t  inode_count;
   uint32_t  block_count;
   uint16_t  block_size;
   uint16_t  maj_ver;
   uint16_t  min_ver;
   uint32_t  allowed_writes;
   uint32_t  write_count;
   uint32_t  mount_POSIX;
   uint32_t  write_POSIX;
   uint32_t  inspect_POSIX;
   uint16_t  state;
   uint16_t  compensation;
   uint8_t   volume_name[VOLUME_NAME_LENGTH];
   uint8_t   mount_dir[MOUNT_DIR_LENGTH];
   uint16_t  inode_size;
   uint32_t  root_index;
   uint32_t  unalloc_index;
   uint32_t  bad_index;
   uint32_t  unalloc_count;
};

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