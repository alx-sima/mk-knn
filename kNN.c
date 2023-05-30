/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "bst.h"
#include "io.h"
#include "point.h"
#include "utils.h"

int node_cmp(void *node1, void *node2, size_t dimension)
{
	struct point *a = node1;
	struct point *b = node2;
	return a->coords[dimension] - b->coords[dimension];
}

struct bst *knn_load_file(char *filename)
{
	FILE *stream = fopen(filename, "rt");
	DIE(!stream, "Unable to open input file");

	int n, k;
	fscanf(stream, "%d%d", &n, &k);

	struct bst *tree = bst_create(k, node_cmp, point_destroy);

	for (int i = 0; i < n; ++i) {
		struct point *p = point_create(k);
		for (int j = 0; j < k; ++j)
			fscanf(stream, "%d", &p->coords[j]);

		bst_insert(tree, p);
		// free(p->coords);
		// free(p);
	}

	fclose(stream);
	return tree;
}

void nearest_neighbor(struct bst *tree, char *args);
void range_search(struct bst *tree, char *args);

int main(void)
{
	char *buffer = NULL;
	size_t buffer_size = 0;

	struct bst *tree = NULL;

	while (read_line(&buffer, &buffer_size, stdin)) {
		char *cmd = strtok(buffer, "\n ");
		char *args = strtok(NULL, "\n");

		if (strcmp(cmd, "LOAD") == 0)
			tree = knn_load_file(args);

		else if (strcmp(cmd, "NN") == 0)
			nearest_neighbor(tree, args);

		else if (strcmp(cmd, "RS") == 0)
			range_search(tree, args);

		else if (strcmp(cmd, "EXIT") == 0)
			break;
	}

	bst_destroy(tree);
	free(buffer);
	return 0;
}
