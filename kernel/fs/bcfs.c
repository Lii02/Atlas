#include <atlas/fs/bcfs.h>
#include <libc/malloc.h>
#include <libc/string.h>

#ifdef I386
#define VGET_BYTE(x) (descriptor[x])
#define VGET_SHORT(x) (descriptor[x] | (descriptor[x + 1] << 8))
#define VGET_INT(x) (descriptor[x] | (descriptor[x + 1] << 8) | (descriptor[x + 2] << 16) | (descriptor[x + 3] << 24))
#endif

void init_pvolume()
{
	memset(&primary_volume_m, 0x0, sizeof(bcfs_volume_t));
	primary_volume_m.dtype = ATA;
	primary_volume_m.ata_device = &ata_primary_master;

	//INFO: Lookat desc.txt for volume descriptor info
	uint8_t descriptor[1024];
	ata_read_sectors(descriptor, 3, 2, &ata_primary_master);
	
	primary_volume_m.sig = VGET_SHORT(0);
	primary_volume_m.inode_count = VGET_INT(2);
	primary_volume_m.block_count = VGET_INT(6);
	primary_volume_m.block_size = VGET_SHORT(10);
	primary_volume_m.major_version = VGET_SHORT(12);
	primary_volume_m.minor_version = VGET_SHORT(14);
	primary_volume_m.num_writes_before = VGET_INT(16);
	primary_volume_m.num_writes_last = VGET_INT(20);
	primary_volume_m.posix_mount = VGET_INT(24);
	primary_volume_m.posix_write = VGET_INT(28);
	primary_volume_m.posix_inspection = VGET_INT(32);
	primary_volume_m.fs_state = VGET_SHORT(36);
	primary_volume_m.error_comp = VGET_SHORT(38);
	int i;
	for(i = 40; i < 56; i++)
	{
		primary_volume_m.volume_name[i - 40] = VGET_BYTE(i);
	}
	for(i = 56; i < 120; i++)
	{
		primary_volume_m.mount_dir[i - 56] = VGET_BYTE(i);
	}
	primary_volume_m.inode_size = VGET_SHORT(120);
	primary_volume_m.root_inode_index = VGET_INT(122);
	primary_volume_m.unallocated_block_index = VGET_INT(126);
	primary_volume_m.bad_block_index = VGET_INT(130);
	primary_volume_m.unallocated_count = VGET_INT(134);
	// 886 bytes are reserved
	
	memset(&primary_volume_s, 0x0, sizeof(bcfs_volume_t));
}

void init_bcfs()
{
	init_pvolume();
}

void free_bcfs()
{
}
