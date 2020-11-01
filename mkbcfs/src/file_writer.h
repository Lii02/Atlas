#include <string>
#include "defs.h"

#ifndef FILE_WRITER_H
#define FILE_WRITER_H

void write_to_img(const std::string& file, const block_t& block);
void write_to_img(const std::string& file, uint_32 byte_count);

void write_uint_8(uint_8 val, uint_8* ptr);
void write_uint_16(uint_16 val, uint_8* ptr);
void write_uint_32(uint_32 val, uint_8* ptr);
void write_string(uint_8* src, uint_8* dest, uint_32 count);

#endif
