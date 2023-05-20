/* Copyright 2023 Alexandru Sima (312CA) */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

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
			return line_len;
		}
	}
	return 0;
}

void load_file(struct trie *dict, char *filename)
{
	FILE *stream = fopen(filename, "r");
	if (!stream) {
		fprintf(stderr, "Error: Unable to open file %s.\n", filename);
		exit(EXIT_FAILURE);
	}

	char *read_buffer = NULL;
	size_t buf_len = 0;

	while (read_line(&read_buffer, &buf_len, stream)) {
		char *word = strtok(read_buffer, " \n");
		while (word) {
			/* TODO: adauga cuvantul in dictionar */
			// puts(word);
			trie_insert(dict, word);
			puts(word);
			word = strtok(NULL, " \n");
		}
	}

	printf("File %s succesfully loaded\n", filename);
	fclose(stream);
	free(read_buffer);
}

void get_first_word();
void get_shortest_word();
void get_most_frequent_word();

void autocomplete(struct trie *dict, char *prefix, int type)
{
	struct node *start = trie_get_prefix(dict, prefix);
	switch (type) {
	case 0:
		print_autocompleted_word(start, prefix, get_first_word);
		print_autocompleted_word(start, prefix, get_shortest_word);
		print_autocompleted_word(start, prefix, get_most_frequent_word);
		break;
	case 1:
		print_autocompleted_word(start, prefix, get_first_word);
		break;
	case 2:
		print_autocompleted_word(start, prefix, get_shortest_word);
		break;
	case 3:
		print_autocompleted_word(start, prefix, get_most_frequent_word);
		break;
	default:
		// fputs("Error: Invalid command.", stderr)
		;
	}
}

int main(void)
{
	char *line = NULL;
	size_t line_size = 0;

	struct trie *dict = trie_create('Z' - 'A' + 1);
	if (!dict) {
		fputs("Error: Unable to create trie.", stderr);
		exit(EXIT_FAILURE);
	}

	while (read_line(&line, &line_size, stdin)) {
		char *cmd = strtok(line, "\n ");
		char *args = strtok(NULL, "\n");

		if (strcmp(cmd, "INSERT") == 0) {
			trie_insert(dict, args);

		} else if (strcmp(cmd, "LOAD") == 0) {
			load_file(dict, args);
			trie_print_all(dict);

		} else if (strcmp(cmd, "REMOVE") == 0) {
			trie_remove(dict, args);

		} else if (strcmp(cmd, "AUTOCORRECT") == 0) {

		} else if (strcmp(cmd, "AUTOCOMPLETE") == 0) {
			char *prefix = strtok(args, "\n ");
			char *type = strtok(NULL, "\n");
			int type_no = atoi(type);
			autocomplete(dict, prefix, type_no);

		} else if (strcmp(cmd, "EXIT") == 0) {
			break;
		} else {
			// fputs("Error: Invalid command.", stderr);
		}
	}

	trie_destroy(dict);
	free(line);
	return 0;
}