/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef BST_H_
#define BST_H_

#include <stddef.h>

#include "point.h"

/**
 * @relates bst
 * @class bst_node
 * @brief Nodul unui arbore binar de cautare multidimensional.
 */
struct bst_node {
	void *data;

	struct bst_node *left;
	struct bst_node *right;
	int level;
};

/**
 * @class bst
 * @brief Arbore binar de cautare multidimensional.
 */
struct bst {
	/** Radacina arborelui */
	struct bst_node *root;
	/** Numarul de dimensiuni ale unui nod */
	size_t dimensions;

	/** Functie de comparatie intre noduri, care
	 * tine cont de nivelul arborelui. */
	int (*node_cmp)(void *node1, void *node2, size_t level);
	/** Destructor de memorie pentru informatiile din noduri. */
	void (*node_destructor)(void *data);
};

/**
 * @relates bst
 * @brief Creaza un arbore binar de cautare multidimensional.
 *
 * @param dimensions		numarul de dimensiuni ale nodurilor
 * @param node_cmp			functie de comparatie intre noduri
 * @param node_destructor	functie de dealocare a nodurilor
 * @return					arborele creat
 */
struct bst *bst_create(size_t dimensions,
					   int (*node_cmp)(void *, void *, size_t),
					   void (*node_destructor)(void *));

/**
 * @relates bst
 * @brief Dealoca un arbore binar de cautare si toate informatiile continute.
 *
 * @param tree arborele de dealocat
 */
void bst_destroy(struct bst *tree);

/**
 * @relates bst
 * @brief Insereaza un nou nod in arbore.
 *
 * @param tree arborele in care se insereaza
 * @param data datele nodului
 */
void bst_insert(struct bst *tree, struct point *data);

#endif /* BST_H_ */
