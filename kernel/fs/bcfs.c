#include <atlas/fs/bcfs.h>
#include <libc/malloc.h>
#include <libc/string.h>

#define VGET_BYTE(x) (descriptor[x])
#define VGET_SHORT(x) (descriptor[x] | (descriptor[x + 1] << 8))

void init_pvolume()
{
	memset(&primary_volume_m, 0x0, sizeof(bcfs_volume_t));
	primary_volume_m.dtype = ATA;
	primary_volume_m.ata_device = &ata_primary_master;

	//INFO: Lookat desc.txt for volume descriptor info
	int8_t descriptor[1024];
	ata_read_sector(descriptor, 3, &ata_primary_master);
	
	primary_volume_m.sig = VGET_SHORT(0);
}

void init_bcfs()
{
	init_pvolume();
}

void free_bcfs()
{
}
