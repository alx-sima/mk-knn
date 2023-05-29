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

long long point_sq_distance(struct point *point_a, struct point *point_b)
{
	long long distance = 0;

	for (size_t i = 0; i < point_a->dimensions; ++i) {
		int diff = point_a->coords[i] - point_b->coords[i];
		distance += diff * diff;
	}

	return distance;
}
