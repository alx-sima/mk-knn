/* Copyright 2023 Alexandru Sima */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

struct bst_node {
	int *data;
	struct bst_node *left;
	struct bst_node *right;
	int level;
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

int point_in_range(int *point, int *range[2], size_t dimensions)
{
	for (size_t i = 0; i < dimensions; ++i) {
		if (point[i] < range[i][0] || point[i] > range[i][1])
			return 0;
	}

	return 1;
}

struct array {
	int **data;
	size_t size;
	size_t capacity;
};

struct array *array_init()
{
	struct array *a = malloc(sizeof(struct array));
	DIE(!a, "failed malloc() of array");

	a->size = 0;
	a->capacity = 1;
	a->data = malloc(sizeof(int *) * a->capacity);
	DIE(!a->data, "failed malloc() of array data");

	return a;
}

void array_push(struct array *a, int *data)
{
	if (a->size == a->capacity) {
		a->data = realloc(a->data, sizeof(int *) * (a->capacity *= 2));
		DIE(!a->data, "failed realloc() of array data");
	}

	a->data[a->size++] = data;
}

void array_destroy(struct array *a)
{
	free(a->data);
	free(a);
}

void bst_range_search(struct bst_node *node, int *range[2], int level, int dim,
					  struct array *found_points)
{
	if (!node)
		return;

	/* Dimensiunea dupa care sunt impartite nodurile la nivelul curent */
	int split_dim = level % dim;

	if (node->data[split_dim] >= range[0][split_dim])
		bst_range_search(node->left, range, level + 1, dim, found_points);

	if (point_in_range(node->data, range, dim)) {
		array_push(found_points, node->data);
	}

	if (node->data[split_dim] <= range[1][split_dim])
		bst_range_search(node->right, range, level + 1, dim, found_points);
}

int points_sort_criterion(const void *a, const void *b)
{
	int *point_a = *(int **)a;
	int *point_b = *(int **)b;

	return 0; // TODO
}

/**
 * @brief Calculeaza norma euclidiana `|| a - b ||`
 */
long sq_distance(int *point_a, int *point_b, size_t dims)
{
	long distance = 0;

	for (size_t i = 0; i < dims; ++i) {
		int diff = point_a[i] - point_b[i];
		distance += diff * diff;
	}

	return distance;
}

void bst_nearest_neighbors(struct bst_node *node, int *coords, int dims,
						   struct array *found_points)
{
	if (!node)
		return;

	int split_dim = node->level % dims;
	if (node->data[split_dim] <= coords[split_dim])
		bst_nearest_neighbors(node->left, coords, dims, found_points);
	else
		bst_nearest_neighbors(node->right, coords, dims, found_points);

	
}

void nearest_neighbor(struct bst *tree, char *args)
{
	int *coords = malloc(tree->data_size);
	DIE(!coords, "failed malloc() of coordinates");

	for (size_t i = 0; i < tree->data_size / sizeof(int); ++i)
		coords[i] = strtol(args, &args, 0);

	struct array *found_points = array_init();
	bst_nearest_neighbors(tree->root, coords, tree->data_size / sizeof(int),
						  found_points);

	array_destroy(found_points);
	free(coords);
}

void range_search(struct bst *tree, char *args)
{
	const size_t dims = tree->data_size / sizeof(int);

	int *ranges[2] = {
		malloc(sizeof(int) * dims),
		malloc(sizeof(int) * dims),
	};
	DIE(!ranges[0], "failed malloc() of ranges buffer");
	DIE(!ranges[1], "failed malloc() of ranges buffer");

	for (size_t i = 0; i < dims; ++i) {
		ranges[0][i] = strtol(args, &args, 0);
		ranges[1][i] = strtol(args, &args, 0);
	}

	struct array *found_points = array_init();
	bst_range_search(tree->root, ranges, 0, dims, found_points);

	// qsort(found_points->data, found_points->size, sizeof(int *),
	//	  points_sort_criterion);

	for (size_t i = 0; i < found_points->size; ++i) {
		for (size_t j = 0; j < dims; ++j)
			printf("%d ", found_points->data[i][j]);
		puts("");
	}

	array_destroy(found_points);
	free(ranges[0]);
	free(ranges[1]);
}
