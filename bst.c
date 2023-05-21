/* Copyright 2023 Alexandru Sima */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

struct bst_node {
	void *data;
	struct bst_node *left;
	struct bst_node *right;
};

struct bst {
	struct bst_node *root;
	size_t data_size;
	int (*node_cmp)(void *node1, void *node2, size_t data_size);
};

struct bst *bst_create(size_t data_size,
					   int (*node_cmp)(void *, void *, size_t))
{
	struct bst *tree = malloc(sizeof(struct bst));
	DIE(!tree, "failed malloc() of tree");

	tree->root = NULL;
	tree->data_size = data_size;
	tree->node_cmp = node_cmp;
	return tree;
}

void bst_node_destroy(struct bst_node *node)
{
	if (!node)
		return;

	bst_node_destroy(node->left);
	bst_node_destroy(node->right);

	free(node->data);
	free(node);
}

void bst_destroy(struct bst *tree)
{
	bst_node_destroy(tree->root);
	free(tree);
}

struct bst_node *bst_node_create(void *data, size_t data_size)
{
	struct bst_node *node = malloc(sizeof(struct bst_node));
	DIE(!node, "failed malloc() of tree node");

	node->data = malloc(data_size);
	DIE(!node->data, "failed malloc() of tree node data");

	memcpy(node->data, data, data_size);
	node->left = NULL;
	node->right = NULL;

	return node;
}

void bst_insert(struct bst *tree, void *data)
{
	struct bst_node *curr = tree->root;

	if (!curr) {
		tree->root = bst_node_create(data, tree->data_size);
		return;
	}

	while (curr) {
		if (tree->node_cmp(data, curr->data, tree->data_size) <= 0) {
			if (!curr->left) {
				curr->left = bst_node_create(data, tree->data_size);
				return;
			}

			curr = curr->left;
		} else {
			if (!curr->right) {
				curr->right = bst_node_create(data, tree->data_size);
				return;
			}

			curr = curr->right;
		}
	}
}
