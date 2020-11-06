#include <atlas/ata.h>
#include <libc/string.h>

#define HDD_IRQ IRQ14
#define HDD2_IRQ IRQ15

struct ata_device_t ata_primary_master = {.base = 0x1F0, .control = 0x3F6, .slave = 0};
struct ata_device_t ata_primary_slave = {.base = 0x1F0, .control = 0x3F6, .slave = 1};

char drive_id = '0';

void hdd_irq_m(cpuregisters_t reg)
{
	CPUINB(ata_primary_master.base + ATA_REG_STATUS);
}

void hdd_irq_s(cpuregisters_t reg)
{
	CPUINB(ata_primary_slave.base + ATA_REG_STATUS);
}

void ata_initialize()
{
	initialize_interrupt(HDD_IRQ, hdd_irq_m);
	initialize_interrupt(HDD2_IRQ, hdd_irq_s);

	ata_detect_device(&ata_primary_master);
	ata_detect_device(&ata_primary_slave);
}

int ata_detect_device(struct ata_device_t* dev)
{
	ata_soft_reset(dev);
	ata_io_wait(dev);
	CPUOUTB(dev->base + ATA_REG_DEVSEL, 0xA0 | (dev->slave << 4));
	ata_io_wait(dev);

	uint8_t cl = CPUINB(dev->base + ATA_REG_LBA1);
	uint8_t ch = CPUINB(dev->base + ATA_REG_LBA2);
	if(cl == 0xFF && ch == 0xFF)
	{
		return 0;
	}
	else if((cl == 0x0 && ch == 0x0) || (cl == 0x3C || ch == 0xC3))
	{
		// ATA device/SATA device
		dev->drivename = strcat("part", &drive_id);
		dev->index = drive_id;

		// Init filesystem/mount here
		ata_init_device(dev);
		bcfs_volume_t* node = ata_create_device(dev);
		
		drive_id++;
		return 1;
	}

	return 0;
}

void ata_io_wait(struct ata_device_t* dev)
{
	// 400ns
	for(int i = 0; i < 4; i++)
		CPUINB(dev->base + ATA_REG_ALTSTATUS);
}

void ata_soft_reset(struct ata_device_t* dev)
{
	CPUOUTB(dev->control, 0x4);
	ata_io_wait(dev);
	CPUOUTB(dev->control, 0x0);
}

bcfs_volume_t* ata_create_device(struct ata_device_t* dev)
{
	bcfs_volume_t* node = (bcfs_volume_t*)malloc(sizeof(bcfs_volume_t));
	memset(node, 0x0, sizeof(bcfs_volume_t));
	node->device = (void*)dev;
	node->index = dev->index;
	node->drivename = dev->drivename;
	node->sig = BCFS_SIGNATURE;
	node->major_version = FS_MAJOR_VERSION;
	node->minor_version = FS_MINOR_VERSION;
	node->unallocated_block_index = -1;
	node->bad_block_index = -1;
	return node;
}

void ata_init_device(struct ata_device_t* dev)
{
	CPUOUTB(dev->base + 1, 1);
	CPUOUTB(dev->control, 0);
	CPUOUTB(dev->base + ATA_REG_DEVSEL, 0xA0 | (dev->slave << 4));
	ata_io_wait(dev);

	CPUOUTB(dev->base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
	ata_io_wait(dev);

	int32_t status = CPUINB(dev->base + ATA_REG_COMMAND);
	dev->status = status;
	ata_io_wait(dev);

	uint16_t* buff = (uint16_t*)&dev->identity;

	for (int i = 0; i < 256; ++i)
	{
		buff[i] = CPUINW(dev->base);
	}

	uint8_t* ptr = (uint8_t*)&dev->identity.model;
	for (int i = 0; i < 39; i += 2)
	{
		uint8_t tmp = ptr[i + 1];
		ptr[i + 1] = ptr[i];
		ptr[i] = tmp;
	}
}
