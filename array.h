/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>

#include "point.h"

struct array {
	// void **data;
	struct point **data;

	size_t data_size;
	size_t size;
	size_t capacity;

	void (*print_element)(void *data);
};

struct array *array_init(size_t data_size, void (*print_element)(void *data));

void array_push(struct array *arr, void *data);

void array_clear(struct array *a);

void array_destroy(struct array *a);

void array_concat(struct array *dest, struct array *src);

void array_print(struct array *a);

#endif /* ARRAY_H_ */
