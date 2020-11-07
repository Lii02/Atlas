#ifndef ATLAS_ATA_H
#define ATLAS_ATA_H
#include <libc/stdint.h>
#include <libc/stdbool.h>
#include <atlas/fs/bcfs_inode.h>
#include <atlas/fs/bcfs_vol.h>
#include <atlas/asm/irq.h>
#ifdef I386
#include <atlas/i386/i386asm.h>
#endif

#define ATA_MASTER 0x0
#define ATA_SLAVE 0x1

#define ATA_REG_DATA 0x0
#define ATA_REG_SECCOUNT0 0x2
#define ATA_REG_LBA0 0x3
#define ATA_REG_LBA1 0x4
#define ATA_REG_LBA2 0x5
#define ATA_REG_LBA3 0x6
#define ATA_REG_DEVSEL 0x6
#define ATA_REG_ALTSTATUS 0x0C
#define ATA_REG_STATUS 0x7
#define ATA_REG_COMMAND 0x7
#define ATA_CMD_READ_PIO 0x20

#define ATA_CMD_IDENTIFY 0xEC

#define ATA_READ 0x0
#define ATA_WRITE 0x1

#define ATA_PRIMARY 0x0
#define ATA_SECONDARY 0x1

#define ATA_SECTOR_SIZES 512
#define BOOTSECTOR_SIZE 1024

struct __attribute__((packed)) ata_identity_t
{
	uint16_t flags;
	uint16_t unused1[9];
	char serial[20];
	uint16_t unused2[3];
	char firmware[8];
	char model[40];
	uint16_t sectors_per_int;
	uint16_t unused3;
	uint16_t capabilities[2];
	uint16_t unused4[2];
	uint16_t valid_ext_data;
	uint16_t unused5[5];
	uint16_t size_of_rw_mult;
	uint32_t sectors_28;
	uint16_t unused6[38];
	uint64_t sectors_48;
	uint16_t unused7[152];
};

struct ata_device_t
{
	int32_t base;
	int16_t control;
	int32_t slave;
	char* drivename;
	int8_t index;
	struct ata_identity_t identity;
	int32_t status;
	int8_t type;
	int8_t precedence;
	void (*read)(uint8_t*, uint32_t, uint32_t, struct ata_device_t*);
};

struct ata_device_t ata_primary_master;
struct ata_device_t ata_primary_slave;

void hdd_irq_m(cpuregisters_t reg);
void hdd_irq_s(cpuregisters_t reg);
void ata_initialize();
int ata_detect_device(struct ata_device_t* dev, int8_t prec, int8_t type);
void ata_io_wait(struct ata_device_t* dev);
void ata_soft_reset(struct ata_device_t* dev);
void ata_init_device(struct ata_device_t* dev);
bool ata_read_sector(uint8_t* buff, uint32_t lba, struct ata_device_t* dev);
void ata_read_sectors(uint8_t* buff, uint32_t lba, uint32_t sects, struct ata_device_t* dev);

#endif
