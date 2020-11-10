#include <atlas/ata.h>
#include <libc/string.h>

#define HDD_IRQ IRQ14
#define HDD2_IRQ IRQ15

#define ATA_PRIMARY_IO 0x1F0
#define ATA_SECONDARY_IO 0x170

#define ATA_IRQ1 14
#define ATA_IRQ2 15

char drive_id = '0';

void hdd_irqp(cpuregisters_t regs)
{
	CPUINB(ATA_PRIMARY_IO + ATA_REG_STATUS);
}

void hdd_irqs(cpuregisters_t regs)
{
	CPUINB(ATA_SECONDARY_IO + ATA_REG_STATUS);
}


void ata_initialize()
{
	initialize_interrupt(ATA_IRQ1, hdd_irqp);
	initialize_interrupt(ATA_IRQ2, hdd_irqs);

	ata_primary_master.base = ATA_PRIMARY_IO;
	ata_primary_master.control = 0x3F6;
	ata_primary_master.slave = ATA_MASTER;

	ata_primary_slave.base = ATA_PRIMARY_IO;
	ata_primary_slave.control = 0x3F6;
   	ata_primary_slave.slave = ATA_SLAVE;

	ata_detect_device(&ata_primary_master);
	ata_io_wait(&ata_primary_master);

	ata_detect_device(&ata_primary_slave);	
	ata_io_wait(&ata_primary_slave);
}

void ide_select_drive(struct ata_device_t* dev)
{
	if(dev->slave)
	{
		CPUOUTB(dev->base + ATA_REG_DEVSEL, 0xB0);
	}
	else
	{
		CPUOUTB(dev->base + ATA_REG_DEVSEL, 0xA0);
	}
}

int ata_detect_device(struct ata_device_t* dev)
{
	ata_soft_reset(dev);
	ide_select_drive(dev);
	ata_io_wait(dev);

	uint8_t cl = CPUINB(dev->base + ATA_REG_LBA1);
	uint8_t ch = CPUINB(dev->base + ATA_REG_LBA2);

	if(cl == 0xFF && ch == 0xFF)
	{
		return false;
	}
	else if((cl == 0x0 && ch == 0x0) || (cl == 0x3C || ch == 0xC3))
	{
		dev->drivename = strcat("part", &(drive_id));
		dev->index = drive_id;
		ata_init_device(dev);
		drive_id++;
	}
	
	return false;
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
	CPUOUTB(dev->control, 0x0);
	ata_io_wait(dev);
}

void ata_init_device(struct ata_device_t* dev)
{
	ide_select_drive(dev);
	ata_io_wait(dev);

	CPUOUTB(dev->base + ATA_REG_SECCOUNT0, 0);
	CPUOUTB(dev->base + ATA_REG_LBA0, 0);
	CPUOUTB(dev->base + ATA_REG_LBA1, 0);
	CPUOUTB(dev->base + ATA_REG_LBA2, 0);
	CPUOUTB(dev->base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

	int8_t status = CPUINB(dev->base + ATA_CMD_IDENTIFY);
	dev->status = status;

	if(status)
	{
		while(CPUINB(dev->base + ATA_REG_STATUS) & ATA_SR_BSY != 0) ;
pm_stat_read:		status = CPUINB(dev->base + ATA_REG_STATUS);
		if(status & ATA_SR_ERR)
		{
			return;
		}
		while(!(status & ATA_SR_DRQ)) goto pm_stat_read;

		int16_t* ibuff = (int16_t*)&dev->identity;
		for(int i = 0; i < 256; i++)
		{
			ibuff[i] = CPUINW(dev->base);
		}

		uint8_t* mptr = (uint8_t*)&dev->identity.model;
		for (int i = 0; i < 39; i += 2)
		{
			uint8_t tmp = mptr[i + 1];
			mptr[i + 1] = mptr[i];
			mptr[i] = tmp;
		}
	}
}

void ide_poll(struct ata_device_t* dev)
{
	ata_io_wait(dev);

	retry:;
	uint8_t status = CPUINB(dev->base + ATA_REG_STATUS);
	if(status & ATA_SR_BSY) goto retry;
retry2:	status = CPUINB(dev->base + ATA_REG_STATUS);
	if(status & ATA_SR_ERR)
	{
		//panic("device failure!\n");
	}
	if(!(status & ATA_SR_DRQ)) goto retry2;
	return;
}

bool ata_read_sector(uint8_t* buff, uint32_t lba, struct ata_device_t* dev)
{
	ide_select_drive(dev);
	ata_io_wait(dev);

	CPUOUTB(dev->base + ATA_REG_SECCOUNT0, dev->identity.sectors_28);

	CPUOUTB(dev->base + ATA_REG_LBA0, (uint8_t)lba);
	CPUOUTB(dev->base + ATA_REG_LBA1, (uint8_t)(lba >> 8));
	CPUOUTB(dev->base + ATA_REG_LBA2, (uint8_t)(lba >> 16));

	CPUOUTB(dev->base + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

	ide_poll(dev);

	for(int i = 0; i < 256; i++)
	{
		uint16_t data = CPUINW(dev->base + ATA_REG_DATA);
		*(uint16_t*)(buff + i * 2) = data;
	}
	ide_poll(dev);
	return true;
}

void ata_read_sectors(uint8_t* buff, uint32_t lba, int32_t num, struct ata_device_t* dev)
{
	for(int i = 0; i < num; i++)
	{
		ata_read_sector(buff, lba + i, dev);
		buff += 512;
	}
}
