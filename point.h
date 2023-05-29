/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef POINT_H_
#define POINT_H_

#include <stddef.h>

#include "utils.h"

/**
 * @class point
 * @brief Structura care reprezinta un punct in spatiu.
 */
struct point {
	/** Numarul de dimensiuni in care se afla punctul */
	size_t dimensions;

	/** Vectorul de coordonate ale punctului */
	int *coords;
};

/**
 * @relates point
 * @brief Creeaza un nou punct.
 *
 * @param dimensions 	numarul de dimensiuni ale punctului
 * @return 				punctul creat
 */
struct point *point_create(size_t dimensions);

/**
 * @relates point
 * @brief Dealoca un punct.
 *
 * @param p punctul de dealocat
 */
void point_destroy(void *p);

/**
 * @relates point
 * @brief Printeaza coordonatele unui punct.
 *
 * @param p punctul de printat
 */
void point_print(void *p);

/**
 * @brief Calculeaza norma euclidiana `|| a - b ||`.
 *
 * @param a	punctul a
 * @param b	punctul b
 * @return 	patratul distantei dintre puncte
 */
long long point_sq_distance(struct point *a, struct point *b);

#endif /* POINT_H_ */
