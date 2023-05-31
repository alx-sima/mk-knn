/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "bst.h"
#include "kNN_queries.h"
#include "point.h"
#include "utils.h"

/**
 * @brief Functie de ordine a 2 puncte pentru arbore.
 *
 * @param node1		primul nod
 * @param node2		al doilea nod
 * @param dimension dimensiunea dupa care se compara nodurile
 * @return			<0 daca primul nod este mai mic decat al doilea
 *					>0 daca primul nod este mai mare decat al doilea
 *					 0 daca nodurile sunt egale
 */
static int node_cmp(void *node1, void *node2, size_t dimension)
{
	struct point *a = node1;
	struct point *b = node2;
	return a->coords[dimension] - b->coords[dimension];
}

/**
 * @brief Construieste un arbore binar de cautare cu datele din fisier.
 *
 * @param filename	fisierul din care se citesc datele
 * @return			arborele construit
 */
static struct bst *knn_load_file(char *filename)
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
		char *cmd = strtok(buffer, " ");
		char *args = strtok(NULL, "");

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
