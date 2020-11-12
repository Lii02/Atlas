#include <atlas/fs/bcfs_table.h>
#include <libc/malloc.h>
#include <libc/string.h>

bcfs_table* bcfs_init_table(int32_t index)
{
	bcfs_table* table = (bcfs_table*)malloc(sizeof(bcfs_table));
	table->index = index;
	for(int i = 0; i < INODES_PER_BLOCK; i++)
	{
		table->inodes[i] = (bcfs_inode_t*)malloc(sizeof(bcfs_inode_t));
	}
	return table;
}

void bcfs_free_table(bcfs_table* table)
{
	for(int i = 0; i < INODES_PER_BLOCK; i++)
	{
		free(table->inodes[i]);
	}
	free(table);
}

void bcfs_set_table_inode(bcfs_table* table, int32_t index, bcfs_inode_t* inode)
{
	table->inodes[index] = inode;
}
