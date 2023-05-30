/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "utils.h"

struct array *array_init(size_t data_size, void (*print_element)(void *),
						 int (*order_func)(const void *, const void *))
{
	struct array *arr = malloc(sizeof(struct array));
	DIE(!arr, "failed malloc() of array");

	arr->data_size = data_size;
	arr->size = 0;
	arr->capacity = 1;
	arr->print_element = print_element;
	arr->order_func = order_func;

	arr->data = malloc(arr->data_size * arr->capacity);
	DIE(!arr->data, "failed malloc() of array data");

	return arr;
}

void array_push(struct array *arr, void *data)
{
	if (arr->size == arr->capacity) {
		arr->data = realloc(arr->data, arr->data_size * (arr->capacity *= 2));
		DIE(!arr->data, "failed realloc() of array data");
	}

	arr->data[arr->size] = malloc(arr->data_size);
	DIE(!arr->data[arr->size], "failed malloc() of array data");

	memcpy(arr->data[arr->size++], data, arr->data_size);
}

void array_clear(struct array *arr)
{
	for (size_t i = 0; i < arr->size; ++i)
		free(arr->data[i]);

	arr->size = 0;
	arr->capacity = 1;
	arr->data = realloc(arr->data, arr->data_size * arr->capacity);
	DIE(!arr->data, "failed realloc() of array data");
}

void array_destroy(struct array *arr)
{
	for (size_t i = 0; i < arr->size; ++i)
		free(arr->data[i]);

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
