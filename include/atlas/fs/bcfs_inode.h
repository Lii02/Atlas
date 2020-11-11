#ifndef BCFS_INODE_H
#define BCFS_INODE_H
#include <libc/stdint.h>

#define UNALLOCATED_INODE 0x0
#define DATA_PTR 0x1
#define INODE_TABLE_PTR 0x2

#define STANDARD_FILE_INODE 0x0
#define DIRECTORY_INODE 0x1

#define USER_READ 0b0000000000000001
#define USER_WRITE 0b0000000000000010
#define USER_EXECUTE 0b0000000000000100
#define USER_MANAGE 0b0000000000001000

#define INODES_PER_BLOCK 16

typedef struct
{
	uint16_t link_type;
	uint16_t allocated_type;
	uint32_t lo_filesize;
	uint32_t hi_filesize;
	uint32_t posix_creation;
	uint32_t posix_access;
	uint32_t posix_modification;
	uint16_t user_id;
	uint16_t permissions;
	uint32_t block_ptr;
	uint8_t info[32];
} bcfs_inode_t;

#endif
