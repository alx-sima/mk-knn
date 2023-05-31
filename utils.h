/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define DIE(assertion, call_description)                                       \
	do {                                                                       \
		if (assertion) {                                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
			perror(call_description);                                          \
			exit(errno);                                                       \
		}                                                                      \
	} while (0)

/**
 * @brief Citeste o linie dintr-un fisier, realocand bufferul daca este necesar.
 *
 * @param[in,out] str	bufferul in care se citeste linia
 * @param[in,out] size	dimensiunea bufferului
 * @return				numarul de caractere citite
 */
size_t read_line(char **str, size_t *size, FILE(*stream));

#endif /* UTILS_H_ */
