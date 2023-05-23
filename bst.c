/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "point.h"
#include "utils.h"

struct bst_node {
	struct point *data;

	struct bst_node *left;
	struct bst_node *right;
	int level;
};

struct bst {
	struct bst_node *root;
	size_t dimensions;
	int (*node_cmp)(void *node1, void *node2, size_t data_size);
};

struct bst *bst_create(size_t dimensions,
					   int (*node_cmp)(void *, void *, size_t))
{
	struct bst *tree = malloc(sizeof(struct bst));
	DIE(!tree, "failed malloc() of tree");

	tree->root = NULL;
	tree->dimensions = dimensions;
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

void bst_insert(struct bst *tree, struct point *data)
{
	struct bst_node *curr = tree->root;

	if (!curr) {
		tree->root = bst_node_create(data, sizeof(struct point));
		return;
	}

	while (curr) {
		if (tree->node_cmp(data, curr->data, tree->dimensions) <= 0) {
			if (!curr->left) {
				curr->left = bst_node_create(data, sizeof(struct point));
				return;
			}

			curr = curr->left;
		} else {
			if (!curr->right) {
				curr->right = bst_node_create(data, sizeof(struct point));
				return;
			}

			curr = curr->right;
		}
	}
}

int point_in_range(struct point *p, int *range[2])
{
	for (size_t i = 0; i < p->dimensions; ++i) {
		if (p->coords[i] < range[0][i] || p->coords[i] > range[1][i])
			return 0;
	}

	return 1;
}

void bst_range_search(struct bst_node *node, int *range[2], int level, int dim,
					  struct array *found_points)
{
	if (!node)
		return;

	/* Dimensiunea dupa care sunt impartite nodurile la nivelul curent */
	int split_dim = level % dim;

	if (node->data->coords[split_dim] >= range[0][split_dim])
		bst_range_search(node->left, range, level + 1, dim, found_points);

	if (point_in_range(node->data, range)) {
		array_push(found_points, node->data);
	}

	if (node->data->coords[split_dim] <= range[1][split_dim])
		bst_range_search(node->right, range, level + 1, dim, found_points);
}

/**
 * @brief Compara 2 puncte, intai dupa prima dimensiune, apoi dupa a 2-a, etc.
 *
 * @param a 	referinta la primul punct
 * @param b 	referinta la punctul al 2-lea
 *
 * @return int	numar care indica relatia de ordine intre a si b
 */
int points_sort_criterion(const void *a, const void *b)
{
	struct point *point_a = *(struct point **)a;
	struct point *point_b = *(struct point **)b;

	for (size_t i = 0; i < point_a->dimensions; ++i) {
		if (point_a->coords[i] != point_b->coords[i])
			return point_a->coords[i] - point_b->coords[i];
	}

	return 0;
}

/**
 * @brief Calculeaza norma euclidiana `|| a - b ||`.
 *
 * @param point_a	punctul a
 * @param point_b	punctul b
 */
long sq_distance(struct point *point_a, struct point *point_b)
{
	long distance = 0;

	for (size_t i = 0; i < point_a->dimensions; ++i) {
		int diff = point_a->coords[i] - point_b->coords[i];
		distance += diff * diff;
	}

	return distance;
}

/**
 * @brief Calculeaza in `found_points` punctele la distanta minima de `coords`
 *
 * @param[in] 	node			nodul curent
 * @param[in] 	coords			punctul de referinta
 * @param[in] 	dims			numarul de dimensiuni ale punctelor
 * @param[out]	found_points	punctele la distanta minima de `coords`
 */
long bst_nearest_neighbors(struct bst_node *node, int *coords, int level,
						   int dims, struct array *found_points)
{
	if (!node)
		return 0xfffff;

	long min_sq_dist;
	int split_dim = level % dims;
	struct bst_node *other_child;

	if (coords[split_dim] <= node->data->coords[split_dim]) {
		min_sq_dist = bst_nearest_neighbors(node->left, coords, level + 1, dims,
											found_points);
		other_child = node->right;
	} else {
		min_sq_dist = bst_nearest_neighbors(node->left, coords, level + 1, dims,
											found_points);
		other_child = node->left;
	}
	struct point *tmp = malloc(sizeof(struct point));
	tmp->dimensions = dims;
	tmp->coords = coords;
	long sq_dist = sq_distance(node->data, tmp);

	long plane_dist = (coords[split_dim] - node->data->coords[split_dim]) *
					  (coords[split_dim] - node->data->coords[split_dim]);
	// TODO: daca distanta de la punctul curent la planul de separare
	// este mai mica decat distanta minima gasita pana acum, atunci
	// trebuie cautat si in celalalt subarbore
	if (plane_dist < min_sq_dist)
		bst_nearest_neighbors(other_child, coords, level + 1, dims,
							  found_points);

	if (sq_dist < min_sq_dist) {
		array_clear(found_points);
		array_push(found_points, node->data);
		min_sq_dist = sq_dist;
	} else if (sq_dist == min_sq_dist) {
		array_push(found_points, node->data);
	}

	return min_sq_dist;
}

/**
 * @brief Afiseaza punctele cele mai apropiate de coordonatele retinute in
 * `args` sub forma de string.
 *
 * @param tree arborele de cautare
 * @param args coordonatele punctului de referinta
 */
void nearest_neighbor(struct bst *tree, char *args)
{
	int *coords = malloc(sizeof(int) * tree->dimensions);
	DIE(!coords, "failed malloc() of coordinates");

	for (size_t i = 0; i < tree->dimensions; ++i)
		coords[i] = strtol(args, &args, 0);

	struct array *found_points = array_init(sizeof(struct point));
	(void)bst_nearest_neighbors(tree->root, coords, 0, tree->dimensions,
								found_points);

	for (size_t i = 0; i < found_points->size; ++i) {
		struct point *p = found_points->data[i];
		for (size_t j = 0; j < p->dimensions; ++j)
			printf("%d ", p->coords[j]);
		puts("");
	}

	array_destroy(found_points);
	free(coords);
}

void range_search(struct bst *tree, char *args)
{
	int *ranges[2] = {
		malloc(sizeof(int) * tree->dimensions),
		malloc(sizeof(int) * tree->dimensions),
	};
	DIE(!ranges[0], "failed malloc() of ranges buffer");
	DIE(!ranges[1], "failed malloc() of ranges buffer");

	for (size_t i = 0; i < tree->dimensions; ++i) {
		ranges[0][i] = strtol(args, &args, 0);
		ranges[1][i] = strtol(args, &args, 0);
	}

	struct array *found_points = array_init(sizeof(struct point));
	bst_range_search(tree->root, ranges, 0, tree->dimensions, found_points);

	// qsort(found_points->data, found_points->size, sizeof(struct point),
	//	  points_sort_criterion);

	for (size_t i = 0; i < found_points->size; ++i) {
		struct point *curr_point = found_points->data[i];
		for (size_t j = 0; j < tree->dimensions; ++j)
			printf("%d ", curr_point->coords[j]);
		puts("");
	}

	array_destroy(found_points);
	free(ranges[0]);
	free(ranges[1]);
}
