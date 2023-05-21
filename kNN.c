/* Copyright 2023 Alexandru Sima */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst.h"
#include "io.h"
#include "utils.h"

int node_cmp(void *node1, void *node2, size_t data_size)
{
	int *a = node1;
	int *b = node2;

	const size_t dims = data_size / sizeof(int);
	for (size_t i = 0; i < dims; ++i) {
		if (a[i] == b[i])
			continue;

		return a[i] - b[i];
	}

	return 0;
}

struct bst *knn_load_file(char *filename)
{
	FILE *stream = fopen(filename, "rt");
	DIE(!stream, "Unable to open input file");

	int n, k;
	fscanf(stream, "%d%d", &n, &k);

	const size_t data_size = sizeof(int) * k;
	struct bst *tree = bst_create(data_size, node_cmp);

	for (int i = 0; i < n; ++i) {
		int *point_coords = malloc(data_size);
		DIE(!point_coords, "failed malloc() of point_coords");

		for (int j = 0; j < k; ++j)
			fscanf(stream, "%d", &point_coords[j]);
		bst_insert(tree, point_coords);
		free(point_coords);
	}

	fclose(stream);
	return tree;
}

int main(void)
{
	char *buffer = NULL;
	size_t buffer_size = 0;

	struct bst *tree = NULL;

	while (read_line(&buffer, &buffer_size, stdin)) {
		char *cmd = strtok(buffer, " \n");
		char *args = strtok(NULL, "\n");

		if (strcmp(cmd, "LOAD") == 0) {
			tree = knn_load_file(args);

		} else if (strcmp(cmd, "EXIT") == 0) {
			break;
		}
	}

	bst_destroy(tree);
	free(buffer);
	return 0;
}
