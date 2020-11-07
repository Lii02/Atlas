#include <atlas/ata.h>
#include <libc/string.h>

#define HDD_IRQ IRQ14
#define HDD2_IRQ IRQ15

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
	ata_primary_master.base = 0x1F0;
	ata_primary_master.control = 0x3F6;
	ata_primary_master.slave = 0;

	ata_primary_slave.base = 0x1F0;
	ata_primary_slave.control = 0x3F6;
   	ata_primary_slave.slave = 1;

	initialize_interrupt(HDD_IRQ, hdd_irq_m);
	initialize_interrupt(HDD2_IRQ, hdd_irq_s);

	ata_detect_device(&ata_primary_master, ATA_PRIMARY, ATA_MASTER);
	ata_primary_master.read = ata_read_sectors;
	ata_io_wait(&ata_primary_master);

	ata_detect_device(&ata_primary_slave, ATA_SECONDARY, ATA_SLAVE);
	ata_primary_slave.read = ata_read_sectors;
	ata_io_wait(&ata_primary_slave);
}

int ata_detect_device(struct ata_device_t* dev, int8_t prec, int8_t type)
{
	ata_soft_reset(dev);
	ata_io_wait(dev);
	CPUOUTB(dev->base + ATA_REG_DEVSEL, 0xA0 | (dev->slave << 4));
	ata_io_wait(dev);
	dev->precedence = prec;
	dev->type = type;

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
	ata_io_wait(dev);
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

bool ata_read_sector(uint8_t* buff, uint32_t lba, struct ata_device_t* dev)
{
	uint8_t cmd = (dev->type == ATA_MASTER ? 0xE0 : 0xF0);
	uint8_t slavebit = (dev->type == ATA_MASTER ? ATA_MASTER : ATA_SLAVE);

	CPUOUTB(dev->base + ATA_REG_DEVSEL, (cmd | (uint8_t)((lba >> 24 & 0x0F))));
	ata_io_wait(dev);
	CPUOUTB(dev->base + 1, 0x00);
	ata_io_wait(dev);
	CPUOUTB(dev->base + ATA_REG_SECCOUNT0, 0x1);
	ata_io_wait(dev);
	CPUOUTB(dev->base + ATA_REG_LBA0, (uint8_t)lba);
	CPUOUTB(dev->base + ATA_REG_LBA1, (uint8_t)(lba >> 8));
	CPUOUTB(dev->base + ATA_REG_LBA2, (uint8_t)(lba >> 16));

	CPUOUTB(dev->base + ATA_REG_COMMAND, ATA_CMD_READ_PIO);
	ata_io_wait(dev);

	for(int i = 0; i < 256; i++)
	{
		uint16_t data = CPUINB(dev->base + ATA_REG_DATA);
		*(uint16_t *)(buff + i * 2) = data;
	}
	ata_io_wait(dev);
	return true;
}

void ata_read_sectors(uint8_t* buff, uint32_t lba, uint32_t sects, struct ata_device_t* dev)
{
	for(int i = 0; i < sects; i++)
	{
		ata_read_sector(buff, lba + i, dev);
		ata_io_wait(dev);
	}
}
