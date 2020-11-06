#ifndef BCFS_VOL_H
#define BCFS_VOL_H
#include <libc/stdint.h>

enum volume_type
{
	ATA = 0x0,
};

#define BCFS_SIGNATURE 0xDC75

#define FS_MAJOR_VERSION 0x1
#define FS_MINOR_VERSION 0x0

#define CLEAN_FS 0x0
#define DIRTY_FS 0x1

#define FS_IGNORE 0x0
#define FS_READ_ONLY 0x1
#define FS_PANIC 0x2

typedef struct
{
	int16_t sig;
	void* device;
	enum volume_type type;
	int8_t index;
	char* drivename;
	int16_t state;
	int16_t error_comp;
	int32_t inode_count;
	int32_t block_count;
	int16_t block_size;
	int16_t major_version;
	int16_t minor_version;
	int16_t inode_size;
	int32_t root_inode_index;
	int32_t unallocated_block_index;
	int32_t bad_block_index;
	int32_t unallocated_count;
	// POSIX stuff would go here
} bcfs_volume_t;

#endif
