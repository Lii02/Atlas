#ifndef ATLAS_HDD_H
#define ATLAS_HDD_H
#include <libc/stdint.h>

struct ata_device_t
{
	int16_t base;
	int16_t dev_ctl;
};

int detect_device_type(int slavebit, struct ata_device_t* device);

#endif
