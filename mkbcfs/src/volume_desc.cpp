#include "volume_desc.h"
#include "file_writer.h"
#include "defs.h"
#include <chrono>
#include <ctime>
#include <string>
#include <string.h>

static volume_desc_t volume;

volume_desc_t& get_volume_desc()
{
   return volume;
}

void init_volume_params(uint_64 volume_size, const std::string& mount_dir)
{
   volume.BCFS_signature = 0xDC75;
   volume.inode_count = 0;
   volume.block_count = (volume_size - 1024) / BLOCK_SIZE;
   volume.block_size = BLOCK_SIZE;
   volume.maj_ver = MAJ_VER;
   volume.min_ver = MIN_VER;
   volume.allowed_writes = ALLOWED_WRITES;
   volume.write_count = 0;
   volume.mount_POSIX = get_POSIX_time();
   volume.write_POSIX = get_POSIX_time();
   volume.inspect_POSIX = get_POSIX_time();
   volume.state = 0;
   volume.compensation = 0;
   strncpy((int_8*) volume.volume_name, "BCFSDefault", (sizeof(volume.volume_name) - 1) / sizeof(volume.volume_name[0]));
   volume.volume_name[(sizeof(volume.volume_name) - 1) / sizeof(volume.volume_name[0])] = 0;
   strncpy((int_8*) volume.mount_dir, mount_dir.c_str(), (sizeof(volume.mount_dir) - 1) / sizeof(volume.mount_dir[0]));
   volume.mount_dir[(sizeof(volume.mount_dir) - 1) / sizeof(volume.mount_dir[0])] = 0;
   volume.inode_size = INODE_SIZE;
   volume.root_index = 0;
   volume.unalloc_index = 0;
   volume.bad_index = 0;
   volume.unalloc_count = volume.block_count;
}

block_t* allocate_volume_desc()
{
   block_t* block = allocate_empty_block();
   write_uint_16(volume.BCFS_signature, &(block->data[0]));
   write_uint_32(volume.inode_count, &(block->data[2]));
   write_uint_32(volume.block_count, &(block->data[6]));
   write_uint_16(volume.block_size, &(block->data[10]));
   write_uint_16(volume.maj_ver, &(block->data[12]));
   write_uint_16(volume.min_ver, &(block->data[14]));
   write_uint_32(volume.allowed_writes, &(block->data[16]));
   write_uint_32(volume.write_count, &(block->data[20]));
   write_uint_32(volume.mount_POSIX, &(block->data[24]));
   write_uint_32(volume.write_POSIX, &(block->data[28]));
   write_uint_32(volume.inspect_POSIX, &(block->data[32]));
   write_uint_16(volume.state, &(block->data[36]));
   write_uint_16(volume.compensation, &(block->data[38]));
   write_string((uint_8*) &(volume.volume_name), &(block->data[40]), VOLUME_NAME_LENGTH);
   write_string((uint_8*) &(volume.mount_dir), &(block->data[56]), MOUNT_DIR_LENGTH);
   write_uint_16(volume.inode_size, &(block->data[120]));
   write_uint_32(volume.root_index, &(block->data[122]));
   write_uint_32(volume.unalloc_index, &(block->data[126]));
   write_uint_32(volume.bad_index, &(block->data[130]));
   write_uint_32(volume.unalloc_count, &(block->data[134]));
   return block;
}

block_t* allocate_empty_block()
{
   return allocate_empty_block(true);
}

block_t* allocate_empty_block(bool is_allocated)
{
   block_t* block = new block_t;
   int i;
   for(i = 0 ; i < BLOCK_SIZE ; ++i) block->data[i] = 0;
   if(is_allocated) volume.unalloc_count--;
   return block;
}

uint_32 get_POSIX_time()
{
   std::tm epoch_start = {};
   epoch_start.tm_sec = 0;
   epoch_start.tm_min = 0;
   epoch_start.tm_hour = 0;
   epoch_start.tm_mday = 1;
   epoch_start.tm_mon = 0;
   epoch_start.tm_year = 70;
   epoch_start.tm_wday = 4;
   epoch_start.tm_yday = 0;
   epoch_start.tm_isdst = -1;
   
   std::time_t base = std::mktime(&epoch_start);
   
   auto diff = std::chrono::system_clock::now() - std::chrono::system_clock::from_time_t(base);
   std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(diff);
   
   return s.count();
}
