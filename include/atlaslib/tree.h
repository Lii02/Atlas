#ifndef ATLAS_TREE_H
#define ATLAS_TREE_H
#include <libc/stdint.h>

struct tree_node_t
{
	size_t size;
	uint32_t id;
	void* data;
	struct tree_node_t* next;
};

typedef struct
{
	uint32_t current_index;
	struct tree_node_t* root;
} tree_t;

tree_t* create_tree();
void free_tree(tree_t* tree);
void free_tree_node(struct tree_node_t* node);
struct tree_node_t* tree_node_create(void* data, uint32_t id);

#endif
