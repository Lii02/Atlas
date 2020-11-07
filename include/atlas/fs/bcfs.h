#ifndef ATLAS_BCFS_H
#define ATLAS_BCFS_H
#include <libc/stdint.h>
#include <atlas/fs/bcfs_inode.h>
#include <atlas/fs/bcfs_vol.h>
#include <atlas/ata.h>

#define BCFS_SECTOR_SIZE 512

#define DEVICE0_ID '0'

// Only works for ATA, for now...
bcfs_volume_t primary_volume_m;
bcfs_volume_t primary_volume_s;

void init_bcfs();
void free_bcfs();

#endif
