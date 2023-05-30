/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "bst.h"
#include "point.h"
#include "utils.h"

/**
 * @relates bst_node
 * @brief Creeaza un nou nod pentru arbore, realizand un shallow copy al
 * informatiei din `data`.
 *
 * @param data	informatia din nod
 * @return 		nodul proaspat creat
 */
static struct bst_node *bst_node_create(struct point *data)
{
	struct bst_node *node = malloc(sizeof(struct bst_node));
	DIE(!node, "failed malloc() of tree node");

	node->data = data;
	node->left = NULL;
	node->right = NULL;

	return node;
}

/**
 * @relates bst_node
 * @brief Sterge recursiv un nod din arbore si descendentii sai.
 *
 * @param node 			nodul curent de sters
 * @param destructor 	functia de eliberare a memoriei pentru informatia
 * 						continuta
 */
static void bst_node_destroy(struct bst_node *node,
							 void (*destructor)(void *data))
{
	if (!node)
		return;

	bst_node_destroy(node->left, destructor);
	bst_node_destroy(node->right, destructor);

	destructor(node->data);
	free(node);
}

struct bst *bst_create(size_t dimensions,
					   int (*node_cmp)(void *, void *, size_t),
					   void (*node_destructor)(void *data))
{
	struct bst *tree = malloc(sizeof(struct bst));
	DIE(!tree, "failed malloc() of tree");

	tree->root = NULL;
	tree->dimensions = dimensions;
	tree->node_cmp = node_cmp;
	tree->node_destructor = node_destructor;
	return tree;
}

void bst_destroy(struct bst *tree)
{
	bst_node_destroy(tree->root, tree->node_destructor);
	free(tree);
}

void bst_insert(struct bst *tree, struct point *data)
{
	struct bst_node *curr = tree->root;
	struct bst_node *new_node = bst_node_create(data);

	if (!curr) {
		tree->root = new_node;
		return;
	}

	size_t level = 0;

	while (curr) {
		size_t split_dim = level % tree->dimensions;
		if (tree->node_cmp(curr->data, data, split_dim) > 0) {
			if (!curr->left) {
				curr->left = new_node;
				return;
			}

			curr = curr->left;
		} else {
			if (!curr->right) {
				curr->right = new_node;
				return;
			}

			curr = curr->right;
		}
		++level;
	}
}
