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

void point_destroy(struct point *p)
{
	free(p->coords);
	free(p);
}
