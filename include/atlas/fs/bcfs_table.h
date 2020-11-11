#ifndef ATLAS_BCFS_TABLE_H
#define ATLAS_BCFS_TABLE_H
#include <libc/stdint.h>
#include "bcfs_inode.h"

typedef struct
{
	size_t length;
	int32_t index;
	bcfs_inode_t* inodes;
} bcfs_table;

bcfs_table* bcfs_init_table(size_t length, int32_t index);
void bcfs_free_table(bcfs_table* table);
void bcfs_set_table_inode(bcfs_table* table, int32_t index, bcfs_inode_t inode);

#endif
