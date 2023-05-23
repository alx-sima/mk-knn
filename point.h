/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef POINT_H_
#define POINT_H_

#include <stddef.h>

#include "utils.h"

struct point {
	int *coords;
	size_t dimensions;
};

struct point *point_create(size_t dimensions);

void point_destroy(struct point *p);

#endif /* POINT_H_ */