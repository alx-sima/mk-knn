/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef BST_H_
#define BST_H_

#include <stddef.h>

#include "point.h"

struct bst;

struct bst *bst_create(size_t dimensions,
					   int (*node_cmp)(void *, void *, size_t));

void bst_destroy(struct bst *tree);

void bst_insert(struct bst *tree, struct point *data);

#endif /* BST_H_ */