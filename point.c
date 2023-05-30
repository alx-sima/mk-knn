/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stdlib.h>

#include "point.h"

struct point *point_create(size_t dimensions)
{
	struct point *p = malloc(sizeof(struct point));
	DIE(!p, "failed malloc() of point");

	p->coords = malloc(sizeof(int) * dimensions);
	DIE(!p->coords, "failed malloc() of point->coords");

	p->dimensions = dimensions;
	return p;
}

void point_destroy(void *p)
{
	struct point *point = p;
	free(point->coords);
	free(point);
}

void point_print(void *p)
{
	struct point *point = p;

	for (size_t i = 0; i < point->dimensions; ++i)
		printf("%d ", point->coords[i]);
	puts("");
}

long long point_sq_distance(struct point *a, struct point *b)
{
	long long distance = 0;

	for (size_t i = 0; i < a->dimensions; ++i) {
		int diff = a->coords[i] - b->coords[i];
		distance += diff * diff;
	}

	return distance;
}

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

int point_in_range(struct point *p, int *range[2])
{
	for (size_t i = 0; i < p->dimensions; ++i) {
		if (p->coords[i] < range[0][i] || p->coords[i] > range[1][i])
			return 0;
	}

	return 1;
}
