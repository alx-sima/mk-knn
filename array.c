/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stddef.h>
#include <stdlib.h>

#include "array.h"
#include "utils.h"

struct array *array_init(size_t data_size, void (*print_element)(void *),
						 int (*order_func)(const void *, const void *))
{
	struct array *a = malloc(sizeof(struct array));
	DIE(!a, "failed malloc() of array");

	a->size = 0;
	a->capacity = 1;
	a->print_element = print_element;
	a->order_func = order_func;

	a->data = malloc(sizeof(void *) * a->capacity);
	DIE(!a->data, "failed malloc() of array data");

	return a;
}

void array_push(struct array *arr, void *data)
{
	if (arr->size == arr->capacity) {
		arr->data = realloc(arr->data, sizeof(void *) * (arr->capacity *= 2));
		DIE(!arr->data, "failed realloc() of array data");
	}

	arr->data[arr->size++] = data;
}

void array_clear(struct array *arr)
{
	arr->size = 0;
	arr->capacity = 1;
	arr->data = realloc(arr->data, sizeof(void *) * arr->capacity);
	DIE(!arr->data, "failed realloc() of array data");
}

void array_destroy(struct array *arr)
{
	free(arr->data);
	free(arr);
}

void array_concat(struct array *dest, struct array *src)
{
	for (size_t i = 0; i < src->size; ++i)
		array_push(dest, src->data[i]);

	array_destroy(src);
}

void array_print(struct array *arr)
{
	for (size_t i = 0; i < arr->size; ++i)
		arr->print_element(arr->data[i]);
}

void array_sort(struct array *arr)
{
	qsort(arr->data, arr->size, sizeof(void *), arr->order_func);
}
