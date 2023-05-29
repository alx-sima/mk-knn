/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef IO_H_
#define IO_H_

#include <stddef.h>
#include <stdio.h>

size_t read_line(char **str, size_t *size, FILE(*stream));

#endif /* IO_H_ */
