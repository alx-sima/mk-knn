/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <limits.h>
#include <stddef.h>

#include "array.h"
#include "kNN_queries.h"
#include "point.h"

/** Macro pentru initializarea unui vector de puncte */
#define CREATE_POINT_ARRAY()                                                   \
	array_init(sizeof(struct point), point_print, points_sort_criterion)

/**
 * @relates bst_node
 * @brief Populeaza `found_points` cu punctele la distanta minima de `coords`.
 *
 * @param[in]	node			nodul curent
 * @param[in]	target			punctul de referinta
 * @param[in]	level			nivelul la care este cautarea curenta
 * @param[out]	found_points	punctele la distanta minima de `coords`
 * @return						distanta minima gasita pana in acest moment
 */
static long long bst_nearest_neighbors(struct bst_node *node,
									   struct point *target, size_t level,
									   struct array *found_points)
{
	if (!node)
		return LLONG_MAX;

	struct point *curr_point = node->data;
	struct bst_node *next_child;
	struct bst_node *other_child;

	size_t split_dim = level % target->dimensions;

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
		struct array *other_points = CREATE_POINT_ARRAY();
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
 * @relates bst
 * @brief Populeaza `found_points` cu punctele care se afla in intervalul
 * `range`.
 *
 * @param node			nodul curent
 * @param range			intervalul de cautare
 * @param level			nivelul la care este cautarea curenta
 * @param dim			dimensiunea spatiului
 * @param found_points	punctele gasite pana in acest moment
 */
static void bst_range_search(struct bst_node *node, int *range[2], size_t level,
							 size_t dim, struct array *found_points)
{
	if (!node)
		return;

	struct point *curr_point = node->data;

	/* Dimensiunea dupa care sunt impartite nodurile la nivelul curent */
	size_t split_dim = level % dim;

	if (point_in_range(node->data, range))
		array_push(found_points, curr_point);

	if (range[0][split_dim] < curr_point->coords[split_dim])
		bst_range_search(node->left, range, level + 1, dim, found_points);

	if (curr_point->coords[split_dim] <= range[1][split_dim])
		bst_range_search(node->right, range, level + 1, dim, found_points);
}

void nearest_neighbor(struct bst *tree, char *args)
{
	struct point *target = point_create(tree->dimensions);

	for (size_t i = 0; i < tree->dimensions; ++i)
		target->coords[i] = strtol(args, &args, 0);

	struct array *found_points = CREATE_POINT_ARRAY();
	(void)bst_nearest_neighbors(tree->root, target, 0, found_points);

	array_sort(found_points);
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

	struct array *found_points = CREATE_POINT_ARRAY();
	bst_range_search(tree->root, ranges, 0, tree->dimensions, found_points);

	array_sort(found_points);
	array_print(found_points);

	array_destroy(found_points);
	free(ranges[0]);
	free(ranges[1]);
}
