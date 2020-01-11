#include "inode.h"
#include "defs.h"
#include "file_writer.h"
#include "volume_desc.h"
#include <iostream>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <fstream>

using namespace std;

static vector<block_t*> data_blocks;
static vector<uint_32> inode_pointers;

static uint_8* prev_inode_ptr = 0;

static uint_32 add_inode(const inode_t& inode)
{
   uint_32 offset = (inode_pointers.size() - 1) * (get_volume_desc().block_size / get_volume_desc().inode_size);
   block_t* block = data_blocks.at(inode_pointers.back());
   uint_8* pointer = &(block->data[0]), *orig_ptr = pointer;
   while(*pointer != 0)
   {
      offset++;
      pointer += get_volume_desc().inode_size;
      if(pointer - orig_ptr == get_volume_desc().block_size - get_volume_desc().inode_size)
      {
         get_volume_desc().inode_count++;
         *pointer = 2;
         write_uint_32(data_blocks.size() + 1, pointer + 28);
         inode_pointers.push_back(data_blocks.size());
         data_blocks.push_back(allocate_empty_block());
         return add_inode(inode);
      }
   }
   write_uint_16(inode.link_type, pointer + 0);
   write_uint_16(inode.alloc_type, pointer + 2);
   write_uint_32(inode.size_lo, pointer + 4);
   write_uint_32(inode.size_hi, pointer + 8);
   write_uint_32(inode.create_POSIX, pointer + 12);
   write_uint_32(inode.access_POSIX, pointer + 16);
   write_uint_32(inode.modify_POSIX, pointer + 20);
   write_uint_16(inode.userID, pointer + 24);
   write_uint_16(inode.permissions, pointer + 26);
   write_uint_32(inode.block_ptr, pointer + 28);
   get_volume_desc().inode_count++;
   prev_inode_ptr = pointer;
   return offset;
}

static uint_32 link_unallocated_blocks()
{
   if(data_blocks.size() >= get_volume_desc().block_count - 1) return 0;
   uint_32 blkqty = get_volume_desc().block_count - (data_blocks.size() + 1), blkptr = data_blocks.size();
   int i;
   for(i = 0 ; i < blkqty ; ++i)
   {
      block_t* block = allocate_empty_block(false);
      data_blocks.push_back(block);
      if(i != blkqty - 1) write_uint_32(data_blocks.size() + 1, &(block->data[get_volume_desc().block_size - 4]));
   }
   return blkptr;
}

static uint_32 allocate_block_chain(uint_8* bytes, uint_64 count)
{
   uint_64 index = 0;
   block_t* block = allocate_empty_block();
   uint_32 blkptr = data_blocks.size();
   data_blocks.push_back(block);
   
   const uint_32 orig_blkptr = blkptr;
   
   while(index < count)
   {
      int i;
      for(i = 0 ; i < get_volume_desc().block_size - 4 ; ++i)
      {
         if(index >= count) break;
         block->data[i] = bytes[index++];
      }
      if(index >= count) break;
      
      block_t* block_new = allocate_empty_block();
      uint_32 blkptr_new = data_blocks.size();
      data_blocks.push_back(block_new);
      write_uint_32(blkptr_new + 1, &(block->data[get_volume_desc().block_size - 4]));
      block = block_new;
      blkptr = blkptr_new;
   }
   
   return orig_blkptr;
}

static uint_32 allocate_block_chain(vector<uint_8>& vec)
{
   uint_32 count = vec.size();
   uint_8* bytes = new uint_8[count];
   int i;
   for(i = 0 ; i < count ; ++i) bytes[i] = vec.at(i);
   uint_32 inode = allocate_block_chain(bytes, count);
   delete[] bytes;
   return inode;
}

static uint_32 read_file(const string& file)
{
   inode_t inode;
   inode.link_type = 0x1;
   inode.alloc_type = 0x0;
   inode.create_POSIX = get_POSIX_time();
   inode.access_POSIX = get_POSIX_time();
   inode.modify_POSIX = get_POSIX_time();
   inode.userID = 0;
   inode.permissions = 4095;
   
   uint_64 filesize = experimental::filesystem::file_size(file);
   inode.size_lo = (filesize >>  0) & 0xFFFFFFFF;
   inode.size_hi = (filesize >> 32) & 0xFFFFFFFF;
   
   ifstream in(file, ios::out | ios::binary);
   uint_8* bytes = new uint_8[filesize];
   
   in.read((int_8*) bytes, filesize);
   inode.block_ptr = filesize == 0 ? 0 : allocate_block_chain(bytes, filesize) + 1;
   in.close();
   delete[] bytes;
   return add_inode(inode);
}

static uint_32 read_directory(const string& dir)
{
   inode_t inode;
   inode.link_type = 0x1;
   inode.alloc_type = 0x1;
   inode.create_POSIX = get_POSIX_time();
   inode.access_POSIX = get_POSIX_time();
   inode.modify_POSIX = get_POSIX_time();
   inode.userID = 0;
   inode.permissions = 4095;
   
   vector<uint_8> bytes;
   for(const auto& entry : experimental::filesystem::directory_iterator(dir))
   {
      uint_32 inode_index;
      if(is_directory(entry.path())) inode_index = read_directory(entry.path().u8string());
      else inode_index = read_file(entry.path().u8string());
      bytes.push_back((inode_index >>  0) & 0xFF);
      bytes.push_back((inode_index >>  8) & 0xFF);
      bytes.push_back((inode_index >> 16) & 0xFF);
      bytes.push_back((inode_index >> 24) & 0xFF);
      
      string name = entry.path().filename().u8string();
      bytes.push_back((name.length() >> 0) & 0xFF);
      bytes.push_back((name.length() >> 8) & 0xFF);
      
      int i;
      for(i = 0 ; i < name.length() & 0xFFFF ; ++i) bytes.push_back(name[i]);
   }
   
   uint_64 size = bytes.size();
   
   inode.size_lo = (size >>  0) & 0xFFFFFFFF;
   inode.size_hi = (size >> 32) & 0xFFFFFFFF;
   inode.block_ptr = size == 0 ? 0 : allocate_block_chain(bytes) + 1;
   return add_inode(inode);
}

vector<block_t*>& mount_filesystem(const string& mntdir)
{
   data_blocks.clear();
   inode_pointers.clear();
   
   data_blocks.push_back(allocate_empty_block());
   inode_pointers.push_back(0);
   
   uint_32 root_index = read_directory(mntdir);
   get_volume_desc().root_index = root_index;
   
   inode_t i_bad;
   i_bad.link_type = 0x1;
   i_bad.alloc_type = 0x0;
   i_bad.create_POSIX = get_POSIX_time();
   i_bad.access_POSIX = get_POSIX_time();
   i_bad.modify_POSIX = get_POSIX_time();
   i_bad.userID = 0;
   i_bad.permissions = 4095;
   i_bad.size_lo = 0;
   i_bad.size_hi = 0;
   i_bad.block_ptr = 0;
   get_volume_desc().bad_index = add_inode(i_bad);
   
   inode_t i_unalloc;
   i_unalloc.link_type = 0x1;
   i_unalloc.alloc_type = 0x0;
   i_unalloc.create_POSIX = get_POSIX_time();
   i_unalloc.access_POSIX = get_POSIX_time();
   i_unalloc.modify_POSIX = get_POSIX_time();
   i_unalloc.userID = 0;
   i_unalloc.permissions = 4095;
   get_volume_desc().unalloc_index = add_inode(i_unalloc);
   
   uint_64 unalloc_byte = ((uint_64) get_volume_desc().block_count - ((uint_64) data_blocks.size() + 1)) * ((uint_64) get_volume_desc().block_size - 4);
   write_uint_32((unalloc_byte >>  0) && 0xFFFFFFFF, prev_inode_ptr + 4);
   write_uint_32((unalloc_byte >> 32) && 0xFFFFFFFF, prev_inode_ptr + 8);
   write_uint_32(data_blocks.size() >= get_volume_desc().block_count - 1 ? 0 : link_unallocated_blocks() + 1, prev_inode_ptr + 28);
   
   return data_blocks;
}
