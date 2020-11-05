#include <atlas/hdd.h>
#include <atlas/asm/irq.h>

#define HDD_IRQ IRQ14
#define REG_CYL_LO 0x4
#define REG_CYL_HI 0x5
#define REG_DEVSEL 0x6
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

int detect_device_type(int slavebit, struct ata_device_t* device)
{
}
