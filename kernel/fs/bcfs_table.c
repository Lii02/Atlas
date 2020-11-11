#include <atlas/fs/bcfs_table.h>
#include <libc/malloc.h>

bcfs_table* bcfs_init_table(size_t length, int32_t index)
{
	bcfs_table* table = (bcfs_table*)malloc(sizeof(bcfs_table));
	table->length = length;
	table->index = index;
	table->inodes = (bcfs_inode_t*)calloc(length, sizeof(bcfs_inode_t));
	return table;
}

void bcfs_free_table(bcfs_table* table)
{
	free(table->inodes);
	free(table);
}

void bcfs_set_table_inode(bcfs_table* table, int32_t index, bcfs_inode_t inode)
{
	table->inodes[index] = inode;
}
