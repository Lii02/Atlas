#include <atlaslib/tree.h>
#include <libc/malloc.h>

tree_t* create_tree()
{
	tree_t* tree = (tree_t*)malloc(sizeof(tree_t));
	tree->root = NULL;
	tree->current_index = 0;
	return tree;
}

void free_tree_node(struct tree_node_t* node)
{
	for(int i = 0; i < node->size; i++)
	{
		free_tree_node(&node->next[i]);
	}
	free(node);
}

void free_tree(tree_t* tree)
{
	if(tree->root)
		free_tree_node(tree->root);
	free(tree);
}

struct tree_node_t* tree_node_create(void* data, uint32_t id)
{
	struct tree_node_t* node = (struct tree_node_t*)malloc(sizeof(struct tree_node_t));
	node->size = 0;
	node->id = id;
	node->data = data;
	return node;
}
