/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

size_t read_line(char **str, size_t *size, FILE *stream)
{
	char buffer[BUFSIZ];

	size_t line_len = 0;
	while (fgets(buffer, BUFSIZ, stream)) {
		size_t buffer_siz = strlen(buffer) + 1;

		/* `str` se realoca doar cand se citeste un
		 * rand mai mare decat precedentele. */
		if (line_len + buffer_siz > *size) {
			*str = realloc(*str, sizeof(char) * (line_len + buffer_siz));
			DIE(!str, "failed realloc() on read buffer");

			*size = line_len + buffer_siz;
		}

		strncpy(*str + line_len, buffer, buffer_siz);
		line_len += buffer_siz - 1;
		if ((*str)[line_len - 1] == '\n') {
			(*str)[line_len - 1] = '\0';
			return line_len;
		}
	}
	return 0;
}
