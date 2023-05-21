/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stdio.h>
#include <string.h>

#include "kbd_functions.h"
#include "trie_node.h"
#include "utils.h"

static int found_word(struct node *n)
{
	return n->words;
}

static int found_frequent_prefix(struct node *n)
{
	return n->words == n->max_freq;
}

static int first_word_advance_condition(struct node *parent, struct node *child)
{
	/* Nodul urmator nu trebuie sa indeplineasca nicio conditie. */
	(void)parent, (void)child;
	return 1;
}

static int shortest_word_advance_condition(struct node *parent,
										   struct node *child)
{
	return child->min_word_len == parent->min_word_len - 1;
}

static int frequent_word_advance_condition(struct node *parent,
										   struct node *child)
{
	return child->max_freq == parent->max_freq;
}

static char *autocomplete_prefix(struct node *n, size_t pos,
								 int (*stop_condition)(struct node *n),
								 int (*advance_condition)(struct node *parent,
														  struct node *child))
{
	if (stop_condition(n)) {
		char *word = malloc(pos + 1);
		DIE(!word, "failed malloc() of word buffer");

		word[pos] = '\0';
		return word;
	}

	for (size_t i = 0; i < 26; ++i) {
		if (!n->children[i])
			continue;

		if (advance_condition(n, n->children[i])) {
			char *word = autocomplete_prefix(n->children[i], pos + 1,
											 stop_condition, advance_condition);
			word[pos] = (char)i + 'a';
			return word;
		}
	}

	return NULL;
}

static void print_autocompleted_word(struct node *n, char *prefix,
									 int (*stop_condition)(struct node *),
									 int (*advance_condition)(struct node *,
															  struct node *))
{
	if (!n) {
		puts("No words found");
		return;
	}

	size_t prefix_len = strlen(prefix);
	char *word =
		autocomplete_prefix(n, prefix_len, stop_condition, advance_condition);

	/* Adauga prefixul la inceputul cuvantului gasit. */
	strncpy(word, prefix, prefix_len);
	puts(word);

	free(word);
}

static int node_autocorrect(struct node *n, char *buffer, size_t pos,
							int changes, int max_changes)
{
	if (buffer[pos] == '\0') {
		if (n->words) {
			puts(buffer);
			return 1;
		}
		return 0;
	}

	char oldchar = buffer[pos];
	int changes_total = 0;
	for (size_t i = 0; i < 26; ++i) {
		if (!n->children[i])
			continue;

		buffer[pos] = i + 'a';
		if (buffer[pos] != oldchar) {
			if (changes != max_changes)
				changes_total += node_autocorrect(
					n->children[i], buffer, pos + 1, changes + 1, max_changes);
			continue;
		}

		changes_total += node_autocorrect(n->children[i], buffer, pos + 1,
										  changes, max_changes);
	}

	buffer[pos] = oldchar;
	return changes_total;
}

void autocomplete(struct trie *dict, char *prefix, int type)
{
	static int (*const stop_conditions[])(struct node *) = {
		found_word,
		found_word,
		found_frequent_prefix,
	};

	static int (*const advance_conditions[])(struct node *, struct node *) = {
		first_word_advance_condition,
		shortest_word_advance_condition,
		frequent_word_advance_condition,
	};

	struct node *start = trie_get_prefix(dict, prefix);

	if (type) {
		print_autocompleted_word(start, prefix, stop_conditions[type - 1],
								 advance_conditions[type - 1]);
		return;
	}

	/* Pentru tipul 0 se afiseaza toate 3 completarile. */
	for (int i = 0; i < 3; ++i)
		print_autocompleted_word(start, prefix, stop_conditions[i],
								 advance_conditions[i]);
}

void autocorrect(struct trie *dict, char *prefix, int max_letters)
{
	if (!node_autocorrect(dict->root, prefix, 0, 0, max_letters))
		puts("No words found");
}