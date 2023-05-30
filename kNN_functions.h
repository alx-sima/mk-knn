/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef KNN_FUNCTIONS_H_
#define KNN_FUNCTIONS_H_

#include "bst.h"

/**
 * @brief Afiseaza punctele cele mai apropiate de coordonatele retinute in
 * `args` sub forma de string.
 *
 * @param tree arborele de cautare
 * @param args coordonatele punctului de referinta, sub forma de string
 */
void nearest_neighbor(struct bst *tree, char *args);

/**
 * @brief Afiseaza punctele din arborele de cautare care se afla in intervalul
 * cautat.
 *
 * @param tree arborele de cautare
 * @param args intervalul in care se efectueaza cautarea, sub forma de string
 */
void range_search(struct bst *tree, char *args);

#endif /* KNN_FUNCTIONS_H_ */
