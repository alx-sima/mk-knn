/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>

struct array {
	void **data;

	size_t data_size;
	size_t size;
	size_t capacity;
};

struct array *array_init(size_t data_size);

void array_push(struct array *arr, void *data);

void array_clear(struct array *a);

void array_destroy(struct array *a);

#endif /* ARRAY_H_ */