/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stddef.h>
#include <stdlib.h>

#include "array.h"
#include "utils.h"

struct array *array_init(size_t data_size)
{
	struct array *a = malloc(sizeof(struct array));
	DIE(!a, "failed malloc() of array");

	a->data_size = data_size;
	a->size = 0;
	a->capacity = 1;
	a->data = malloc(a->data_size * a->capacity);
	DIE(!a->data, "failed malloc() of array data");

	return a;
}

void array_push(struct array *a, void *data)
{
	if (a->size == a->capacity) {
		a->data = realloc(a->data, a->data_size * (a->capacity *= 2));
		DIE(!a->data, "failed realloc() of array data");
	}

	a->data[a->size++] = data;
}

void array_clear(struct array *a)
{
	a->size = 0;
	a->capacity = 1;
	a->data = realloc(a->data, a->data_size * a->capacity);
	DIE(!a->data, "failed realloc() of array data");
}

void array_destroy(struct array *a)
{
	free(a->data);
	free(a);
}