#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include <experimental/filesystem>
#include "volume_desc.h"
#include "file_writer.h"
#include "inode.h"
#include "defs.h"

using namespace std;

/*
 * A short program that can mount a folder to a BCFS filesystem
 * and generate a virtual disk image (.img) file.
 */

int main(int argc, char** argv)
{
   if(argc != 4)
   {
      cout << "mkbcfs - usage:" << endl;
      cout << "mkbcfs \"[disk image output]\" \"[mount folder]\" [volume size]" << endl;
      cout << "\tDisk image output: .img file to write the disk image to" << endl;
      cout << "\tMount folder: folder that serves as the root directory of the filesystem" << endl;
      cout << "\tVolume size: size (in bytes) of the disk image volume" << endl;
   }
   else
   {
      stringstream ss;
      ss << experimental::filesystem::current_path().u8string() << "/" << string(argv[2]);

      stringstream ss0;
      ss0 << experimental::filesystem::current_path().u8string() << "/" << string(argv[1]);
      
      string mntdir = ss.str();
      string diskimg = ss0.str();
      uint_64 volume_size = stoul(string(argv[3]), nullptr, 0);
      
      init_volume_params(volume_size, mntdir);
      
      vector<block_t*>& blocks = mount_filesystem(mntdir);
      if((uint_64) (blocks.size() + 1) * (uint_64) BLOCK_SIZE + (uint_64) 1024 > volume_size)
      {
         cout << "mkbcfs - FATAL ERR: Not enough disk space for all elements in mount directory" << endl;
         cout << "mkbcfs - STATUS: Operation failure" << endl;
         int i;
         for(i = 0 ; i < blocks.size() ; ++i) delete blocks.at(i);
         return -1;
      }
      
      block_t* volume_desc = allocate_volume_desc();
      
      remove(diskimg.c_str());
      write_to_img(diskimg, 1024); // reserved boot block
      write_to_img(diskimg, *volume_desc);
      
      delete volume_desc;
      
      int i;
      for(i = 0 ; i < blocks.size() ; ++i)
      {
         write_to_img(diskimg, *(blocks.at(i)));
         delete blocks.at(i);
      }
      for( ; i < get_volume_desc().block_count ; ++i) write_to_img(diskimg, 1024);
      cout << "mkbcfs - STATUS: Operation complete" << endl;
   }
}
