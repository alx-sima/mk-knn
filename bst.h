/* Copyright 2023 Alexandru Sima */

#ifndef BST_H_
#define BST_H_

#include <stddef.h>

struct bst;

struct bst *bst_create(size_t data_size,
					   int (*node_cmp)(void *, void *, size_t));

void bst_destroy(struct bst *tree);

void bst_insert(struct bst *tree, void *data);

#endif /* BST_H_ */