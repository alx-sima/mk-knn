/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mk_queries.h"
#include "trie.h"
#include "utils.h"

/** Lungimea alfabetului limbii engleze */
#define ALPHABET_SIZE ('z' - 'a' + 1)

/**
 * @brief Incarca toate cuvintele din fisierul dat in dictionar.
 *
 * @param dict      trie in care se stocheaza cuvintele
 * @param filename  calea catre fisierul input
 */
static void mk_load_file(struct trie *dict, char *filename)
{
	FILE *stream = fopen(filename, "rt");
	DIE(!stream, "Unable to open input file");

	char *read_buffer = NULL;
	size_t buf_len = 0;

	while (read_line(&read_buffer, &buf_len, stream)) {
		/* Se separa cuvintele dupa whitespaceuri
		 * si se insereaza fiecare pe rand */
		char *word = strtok(read_buffer, " \t\n");
		while (word) {
			trie_insert(dict, word);
			word = strtok(NULL, " \t\n");
		}
	}

	fclose(stream);
	free(read_buffer);
}

int main(void)
{
	char *line = NULL;
	size_t line_size = 0;

	struct trie *dict = trie_create(ALPHABET_SIZE);

	while (read_line(&line, &line_size, stdin)) {
		char *cmd = strtok(line, "\n ");
		char *args = strtok(NULL, "\n");

		if (strcmp(cmd, "INSERT") == 0) {
			trie_insert(dict, args);

		} else if (strcmp(cmd, "LOAD") == 0) {
			mk_load_file(dict, args);

		} else if (strcmp(cmd, "REMOVE") == 0) {
			trie_remove(dict, args);

		} else if (strcmp(cmd, "AUTOCORRECT") == 0) {
			char *prefix = strtok(args, "\n ");
			char *type = strtok(NULL, "\n");
			int chars = atoi(type);
			autocorrect(dict, prefix, chars);

		} else if (strcmp(cmd, "AUTOCOMPLETE") == 0) {
			char *prefix = strtok(args, "\n ");
			char *type = strtok(NULL, "\n");
			int type_no = atoi(type);
			autocomplete(dict, prefix, type_no);

		} else if (strcmp(cmd, "EXIT") == 0) {
			break;

		} else {
			fputs("Error: Invalid command.", stderr);
		}
	}

	trie_destroy(dict);
	free(line);
	return 0;
}
