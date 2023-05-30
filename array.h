/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>

#include "point.h"

/**
 * @class array
 * @brief Un vector care retine referinte la tipuri de date arbitrare.
 */
struct array {
	/** Vectorul cu elementele stocate */
	// void **data;
	struct point **data;

	/** Numarul de elemente stocate */
	size_t size;
	/** Capacitatea efectiva de stocare */
	size_t capacity;

	/** Functie de afisat un element stocat */
	void (*print_element)(void *data);

	/** Functie de comparare a doua elemente stocate */
	int (*order_func)(const void *a, const void *b);
};

/**
 * @relates array
 * @brief
 *
 * @param data_size
 * @param print_element
 * @param order_func
 * @return
 */
struct array *array_init(size_t data_size, void (*print_element)(void *data),
						 int (*order_func)(const void *a, const void *b));

/**
 * @relates array
 * @brief Adauga un element la sfarsitul vectorului.
 *
 * @param arr  	vectorul
 * @param data	elementul de adaugat
 */
void array_push(struct array *arr, void *data);

/**
 * @relates array
 * @brief Elimina toate elementele din vector.
 *
 * @param arr vectorul
 */
void array_clear(struct array *arr);

/**
 * @relates array
 * @brief Elimina toate elementele din vector si il dealoca pe acesta.
 *
 * @param arr vectorul
 */
void array_destroy(struct array *arr);

/**
 * @relates array
 * @brief Adauga toate elementele din `src` la sfarsitul lui `dest` si distruge
 * vectorul `src`.
 *
 * @param dest vectorul destinatie
 * @param src  vectorul sursa
 */
void array_concat(struct array *dest, struct array *src);

/**
 * @relates array
 * @brief Sorteaza vectorul, folosind functia interna de comparatie.
 *
 * @param arr vectorul
 */
void array_sort(struct array *arr);

/**
 * @relates array
 * @brief Afiseaza vectorul, folosind functia interna de afisare.
 *
 * @param arr vectorul
 */
void array_print(struct array *arr);

#endif /* ARRAY_H_ */
