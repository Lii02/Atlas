#include <atlas/fs/bcfs.h>
#include <libc/malloc.h>
#include <libc/string.h>

void init_pvolume()
{
	memset(&primary_volume_m, 0x0, sizeof(bcfs_volume_t));
	primary_volume_m.dtype = ATA;
	primary_volume_m.ata_device = &ata_primary_master;

	char descriptor[1024];
	ata_read_sectors(descriptor, 2, 2, &ata_primary_master);
	int8_t* ptr = descriptor;

#define GET_SHORT() ((uint16_t)(*((uint16_t*)(ptr))))
#define GET_INT() ((uint32_t)(*((uint32_t*)(ptr))))
#define GET_BYTE() ((uint8_t)*(ptr))
#define INCREASE_PTR(x) for(int i = 0; i < x; i++) *(ptr)++;

	primary_volume_m.sig = GET_SHORT();
	INCREASE_PTR(2);

	primary_volume_m.inode_count = GET_INT();
	INCREASE_PTR(4);
	primary_volume_m.block_count = GET_INT();
	INCREASE_PTR(4);

	primary_volume_m.block_size = GET_SHORT();
	INCREASE_PTR(2);

	primary_volume_m.major_version = GET_SHORT();
	INCREASE_PTR(2);	
	primary_volume_m.minor_version = GET_SHORT();
	INCREASE_PTR(2);

	memset(&primary_volume_s, 0x0, sizeof(bcfs_volume_t));

#undef GET_SHORT
#undef GET_INT
#undef GET_BYTE
#undef INCREASE_PTR
}

void init_bcfs()
{
	init_pvolume();
}

void free_bcfs()
{
}
