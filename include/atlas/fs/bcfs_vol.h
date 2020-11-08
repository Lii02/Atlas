#ifndef BCFS_VOL_H
#define BCFS_VOL_H
#include <libc/stdint.h>
#include <atlas/ata.h>

typedef enum
{
	ATA = 0x0,
} device_type;

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
	union
	{
		device_type dtype;

		struct
		{
			struct ata_device_t* ata_device;
		}
	};	

	int16_t sig;
	int16_t fs_state;
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
	int32_t posix_mount;
	int32_t posix_write;
	int32_t posix_inspection;
	int32_t num_writes_before;
	int32_t num_writes_last;
	int32_t fs_allocated_blocks;
	char volume_name[16];
	char mount_dir[64];
	char unused0[886];
} bcfs_volume_t;

#endif
