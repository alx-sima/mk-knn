/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
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

	free(node->data->coords);
	free(node->data);
	free(node);
}

void bst_destroy(struct bst *tree)
{
	bst_node_destroy(tree->root);
	free(tree);
}

struct bst_node *bst_node_create(struct point *data, size_t data_size)
{
	struct bst_node *node = malloc(sizeof(struct bst_node));
	DIE(!node, "failed malloc() of tree node");

	node->data = data;
	// node->data = malloc(data_size);
	DIE(!node->data, "failed malloc() of tree node data");

	// memcpy(node->data, data, data_size);
	node->left = NULL;
	node->right = NULL;

	return node;
}

void bst_insert(struct bst *tree, struct point *data)
{
	struct bst_node *curr = tree->root;
	struct bst_node *new_node = bst_node_create(data, sizeof(struct point));

	if (!curr) {
		tree->root = new_node;
		return;
	}

	int level = 0;

	while (curr) {
		int split_dim = level % tree->dimensions;

		if (curr->data->coords[split_dim] > data->coords[split_dim]) {
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

	if (point_in_range(node->data, range))
		array_push(found_points, node->data);

	if (range[0][split_dim] < node->data->coords[split_dim])
		bst_range_search(node->left, range, level + 1, dim, found_points);

	if (node->data->coords[split_dim] <= range[1][split_dim])
		bst_range_search(node->right, range, level + 1, dim, found_points);
}

/**
 * @brief Compara 2 puncte, intai dupa prima dimensiune, apoi dupa a 2-a, etc.
 *
 * @param a		referinta la primul punct
 * @param b		referinta la punctul al 2-lea
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
 * @brief Calculeaza in `found_points` punctele la distanta minima de `coords`
 *
 * @param[in]	node			nodul curent
 * @param[in]	target			punctul de referinta
 * @param[in]	dims			numarul de dimensiuni ale punctelor
 * @param[out]	found_points	punctele la distanta minima de `coords`
 */
long long bst_nearest_neighbors(struct bst_node *node, struct point *target,
								int level, struct array *found_points)
{
	if (!node)
		return LLONG_MAX;

	struct point *curr_point = node->data;
	struct bst_node *next_child;
	struct bst_node *other_child;

	int split_dim = level % target->dimensions;

	if (target->coords[split_dim] <= curr_point->coords[split_dim]) {
		next_child = node->left;
		other_child = node->right;
	} else {
		next_child = node->right;
		other_child = node->left;
	}

	long long best_distance =
		bst_nearest_neighbors(next_child, target, level + 1, found_points);

	long long plane_dist =
		target->coords[split_dim] - curr_point->coords[split_dim];

	long long sq_point_dist = point_sq_distance(node->data, target);

	if (sq_point_dist < best_distance) {
		array_clear(found_points);
		array_push(found_points, curr_point);
		best_distance = sq_point_dist;

	} else if (sq_point_dist == best_distance) {
		array_push(found_points, curr_point);
	}

	/* Daca punctul curent e mai aproape de planul de separare decat
	 * distanta minima gasita pana acum, atunci trebuie sa cautam si in
	 * celalalt subarbore, pentru ca pot exista puncte mai apropiate acolo */
	if (best_distance >= plane_dist * plane_dist) {
		struct array *other_points =
			array_init(sizeof(struct point), point_print);
		long long other_best_distance =
			bst_nearest_neighbors(other_child, target, level + 1, other_points);

		if (other_best_distance < best_distance) {
			best_distance = other_best_distance;
			array_clear(found_points);
			array_concat(found_points, other_points);
		} else if (other_best_distance == best_distance) {
			array_concat(found_points, other_points);
		} else {
			array_destroy(other_points);
		}
	}

	return best_distance;
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
	struct point *target = point_create(tree->dimensions);

	for (size_t i = 0; i < tree->dimensions; ++i)
		target->coords[i] = strtol(args, &args, 0);

	struct array *found_points = array_init(sizeof(struct point), point_print);
	(void)bst_nearest_neighbors(tree->root, target, 0, found_points);

	qsort(found_points->data, found_points->size, sizeof(void *),
		  points_sort_criterion);

	array_print(found_points);

	array_destroy(found_points);
	point_destroy(target);
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

	struct array *found_points = array_init(sizeof(struct point), point_print);
	bst_range_search(tree->root, ranges, 0, tree->dimensions, found_points);

	qsort(found_points->data, found_points->size, sizeof(void *),
		  points_sort_criterion);

	array_print(found_points);

	array_destroy(found_points);
	free(ranges[0]);
	free(ranges[1]);
}
