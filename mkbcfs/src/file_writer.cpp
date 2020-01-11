#include "file_writer.h"
#include "defs.h"
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

void write_to_img(const string& file, const block_t& block)
{
   FILE* pFile;
   pFile = fopen(file.c_str(), "ab");
   
   if(pFile == 0)
   {
      cout << "mkbcfs - STD ERROR: Could not open output image \'" << file << "\'" << endl;
      return;
   }
   
   fwrite(&(block.data), 1, BLOCK_SIZE * sizeof(uint_8), pFile);
   
   fclose(pFile);
}

void write_to_img(const string& file, uint_32 byte_count)
{
   FILE* pFile;
   pFile = fopen(file.c_str(), "ab");
   
   if(pFile == 0)
   {
      cout << "mkbcfs - STD ERROR: Could not open output image \'" << file << "\'" << endl;
      return;
   }
   
   uint_8 data[byte_count];
   int i;
   for(i = 0 ; i < byte_count ; ++i) data[i] = 0;
   
   fwrite(&data, 1, byte_count * sizeof(uint_8), pFile);
   
   fclose(pFile);
}

void write_uint_8(uint_8 val, uint_8* ptr)
{
   *ptr = val;
}

void write_uint_16(uint_16 val, uint_8* ptr)
{
   *ptr = val & 0xFF;
   *(ptr + 1) = (val >> 8) & 0xFF;
}

void write_uint_32(uint_32 val, uint_8* ptr)
{
   *ptr = val & 0xFF;
   *(ptr + 1) = (val >> 8) & 0xFF;
   *(ptr + 2) = (val >> 16) & 0xFF;
   *(ptr + 3) = (val >> 24) & 0xFF;
}

void write_string(uint_8* src, uint_8* dest, uint_32 count)
{
   int i;
   for(i = 0 ; i < count ; ++i)
   {
      *(dest + i) = *(src + i);
   }
}
