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
 * @relates point
 * @brief Calculeaza norma euclidiana `|| a - b ||`.
 *
 * @param a	punctul a
 * @param b	punctul b
 * @return 	patratul distantei dintre puncte
 */
long long point_sq_distance(struct point *a, struct point *b);

/**
 * @relates point
 * @brief Compara 2 puncte, intai dupa prima dimensiune, apoi dupa a 2-a, etc.
 *
 * @param a		referinta la primul punct
 * @param b		referinta la punctul al 2-lea
 * @return 		numar care indica relatia de ordine intre a si b (pentru qsort)
 */
int points_sort_criterion(const void *a, const void *b);

/**
 * @relates point
 * @brief Verifica daca un punct se afla in intervalul dat.
 *
 * @param p 	punctul
 * @param range intervalul in care se cauta.
 * 					range[0] reprezinta capatul inferior,
 * 					iar range[1] capatul superior
 * @return		1 daca punctul se afla in interval, 0 altfel
 */
int point_in_range(struct point *p, int *range[2]);

#endif /* POINT_H_ */
