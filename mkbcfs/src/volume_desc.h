#include <string>
#include "defs.h"

#ifndef VOLUME_DESC_H
#define VOLUME_DESC_H

#define VOLUME_NAME_LENGTH 16
#define MOUNT_DIR_LENGTH 64

typedef struct
{
   uint_16  BCFS_signature;
   uint_32  inode_count;
   uint_32  block_count;
   uint_16  block_size;
   uint_16  maj_ver;
   uint_16  min_ver;
   uint_32  allowed_writes;
   uint_32  write_count;
   uint_32  mount_POSIX;
   uint_32  write_POSIX;
   uint_32  inspect_POSIX;
   uint_16  state;
   uint_16  compensation;
   uint_8   volume_name[VOLUME_NAME_LENGTH];
   uint_8   mount_dir[MOUNT_DIR_LENGTH];
   uint_16  inode_size;
   uint_32  root_index;
   uint_32  unalloc_index;
   uint_32  bad_index;
   uint_32  unalloc_count;
} volume_desc_t;

volume_desc_t& get_volume_desc();
void init_volume_params(uint_64 volume_size, const std::string& mount_dir);
block_t* allocate_volume_desc();

block_t* allocate_empty_block();
block_t* allocate_empty_block(bool is_allocated);
uint_32 get_POSIX_time();

#endif
