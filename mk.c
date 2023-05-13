/* Copyright 2023 Sima Alexandru (312CA) */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t read_line(char **str, size_t *size, FILE *stream)
{
	char buffer[BUFSIZ];

	size_t line_len = 0;
	while (fgets(buffer, BUFSIZ, stream)) {
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

void load_file(char *filename)
{
	FILE *stream = fopen(filename, "r");
	if (!stream) {
		fprintf(stderr, "Error: Unable to open file %s.\n", filename);
		exit(EXIT_FAILURE);
	}

	char *read_buffer = NULL;
	size_t buf_len = 0;

	while (read_line(&read_buffer, &buf_len, stream)) {
		char *word = strtok(read_buffer, " ");
		while (word) {
			/* TODO: adauga cuvantul in dictionar */
			puts(word);

			word = strtok(NULL, " ");
		}
	}

	printf("File %s succesfully loaded\n", filename);
	fclose(stream);
	free(read_buffer);
}

int main(void)
{
	char *line = NULL;
	size_t line_size = 0;

	while (read_line(&line, &line_size, stdin)) {
		char *cmd = strtok(line, " ");
		char *args = strtok(NULL, "");

		if (strcmp(cmd, "INSERT") == 0) {

		} else if (strcmp(cmd, "LOAD") == 0) {
			load_file(args);
		}
	}

	free(line);
	return 0;
}