/* Copyright 2023 Sima Alexandru (312CA) */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t read_line(char **str, size_t *size)
{
	char buffer[BUFSIZ];

	size_t line_len = 0;
	while (fgets(buffer, BUFSIZ, stdin)) {
		size_t buffer_siz = strlen(buffer) + 1;

		/* `str` se realoca doar cand se citeste un
		 * rand mai mare decat precedentele. */
		if (line_len + buffer_siz > *size) {
			char *tmp = realloc(*str, sizeof(char) * (line_len + buffer_siz));
			if (!tmp)
				return 0;
			*size = line_len + buffer_siz;
			*str = tmp;
		}

		strncpy(*str + line_len, buffer, buffer_siz);
		line_len += buffer_siz - 1;
		if ((*str)[line_len - 1] == '\n') {
			(*str)[line_len - 1] = '\0';
			return line_len - 1;
		}
	}
	return 0;
}

int main(void)
{
	char *line = NULL;
	size_t line_size = 0;

	while (read_line(&line, &line_size)) {
		char *cmd = strtok(line, " ");
		char *args = strtok(NULL, "");

		if (strcmp(cmd, "INSERT") == 0) {

		} else if (strcmp(cmd, "LOAD") == 0) {
			puts("File succesfully loaded");
		}
	}

	free(line);
	return 0;
}