/* Copyright 2023 Alexandru Sima (312CA) */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "trie_internals.h"

struct trie *trie_create(size_t alphabet_size)
{
	struct trie *t = malloc(sizeof(struct trie));
	if (!t)
		return NULL;

	t->alphabet_size = alphabet_size;

	t->root = node_create(alphabet_size);
	if (!t->root) {
		free(t);
		return NULL;
	}

	return t;
}

void trie_destroy(struct trie *t)
{
	node_destroy(t->root, t->alphabet_size);
	free(t);
}

void trie_insert(struct trie *t, char *word)
{
	size_t len_word = strlen(word);
	node_insert(t->root, word, len_word);
}

int trie_remove(struct trie *t, char *word)
{
	return node_remove(&t->root, word);
}

struct node *get_prefix(struct node *n, char *prefix)
{
	if (*prefix == '\0')
		return n;

	size_t index = *prefix - 'a';
	if (!n->children[index])
		return NULL;

	return get_prefix(n->children[index], prefix + 1);
}

struct node *trie_get_prefix(struct trie *t, char *prefix)
{
	return get_prefix(t->root, prefix);
}

char *get_first_word(struct node *n, size_t pos)
{
	if (n->words) {
		char *word = malloc(pos + 1);
		word[pos] = '\0';
		return word;
	}
	for (size_t i = 0; i < 26; ++i) {
		if (n->children[i]) {
			char *word = get_first_word(n->children[i], pos + 1);
			word[pos] = (char)i + 'a';
			return word;
		}
	}

	return NULL;
}

char *get_shortest_word(struct node *n, size_t pos)
{
	if (n->words) {
		char *word = malloc(pos + 1);
		word[pos] = '\0';
		return word;
	}

	for (size_t i = 0; i < 26; ++i) {
		if (!n->children[i])
			continue;

		if (n->children[i]->min_word_len == n->min_word_len - 1) {
			char *word = get_shortest_word(n->children[i], pos + 1);
			word[pos] = i + 'a';
			return word;
		}
	}

	return NULL;
}

char *get_most_frequent_word(struct node *n, size_t pos)
{
	if (n->words == n->max_freq) {
		char *word = malloc(pos + 1);
		word[pos] = '\0';
		return word;
	}

	for (size_t i = 0; i < 26; ++i) {
		if (!n->children[i])
			continue;

		if (n->children[i]->max_freq == n->max_freq) {
			char *word = get_most_frequent_word(n->children[i], pos + 1);
			word[pos] = i + 'a';
			return word;
		}
	}

	return NULL;
}

void print_first_word(struct node *n, char *prefix)
{
	if (!n) {
		puts("No word found");
		return;
	}

	char *word = get_first_word(n, strlen(prefix));
	puts(word);
	free(word);
}

void print_autocompleted_word(struct node *n, char *prefix,
							  char *(*autocomplete_func)(struct node *, size_t))
{
	if (!n) {
		puts("No word found.");
		return;
	}

	size_t prefix_len = strlen(prefix);

	char *word = autocomplete_func(n, prefix_len);
	strncpy(word, prefix, prefix_len);
	puts(word);
	free(word);
}

void print_most_frequent_word(struct node *n, char *prefix)
{
	if (!n) {
		puts("No word found.");
		return;
	}

	char *word = get_shortest_word(n, strlen(prefix));
	puts(word);
	free(word);
}

// ! DEPRECATED
void node_print_all(struct node *t)
{
	if (t->words) {
		printf(": %zu\n", t->words);
	}
	for (size_t i = 0; i < 26; ++i) {
		if (t->children[i] != NULL) {
			printf("%c", (char)i + 'a');
			node_print_all(t->children[i]);
		}
	}
}

void trie_print_all(struct trie *t)
{
	node_print_all(t->root);
}