/* Copyright 2023 Alexandru Sima (312CA) */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "trie.h"

struct trie_node *node_create(size_t alphabet_size)
{
	struct trie_node *n = malloc(sizeof(struct trie_node));
	if (!n)
		return NULL;

	n->children = calloc(alphabet_size, sizeof(struct trie_node *));
	if (!n->children) {
		free(n);
		return NULL;
	}

	n->children_no = 0;
	n->words = 0;
	return n;
}

struct trie *trie_create(size_t alphabet_size)
{
	struct trie *t = malloc(sizeof(struct trie));
	if (!t)
		return NULL;

	t->roots = calloc(alphabet_size, sizeof(struct trie_node *));
	if (!t->roots) {
		free(t);
		return NULL;
	}

	t->alphabet_size = alphabet_size;
	return t;
}

void trie_node_destroy(struct trie_node *n, size_t alphabet_size)
{
	for (size_t i = 0; i < alphabet_size; ++i) {
		if (n->children[i])
			trie_node_destroy(n->children[i], alphabet_size);
	}

	free(n->children);
	free(n);
}

void trie_destroy(struct trie *t)
{
	for (size_t i = 0; i < t->alphabet_size; ++i) {
		if (t->roots[i])
			trie_node_destroy(t->roots[i], t->alphabet_size);
	}

	free(t->roots);
	free(t);
}

void trie_node_insert(struct trie_node *t, char *key)
{
	if (*key == '\0') {
		++t->words;
		return;
	}
	size_t char_index = *key - 'a';
	if (t->children[char_index] == NULL) {
		++t->children_no;
		t->children[char_index] = node_create(26);
	}
	// TODO programare defensiva
	trie_node_insert(t->children[char_index], key + 1);
}

void trie_insert(struct trie *t, char *key)
{
	char current_char = *key;
	size_t index = current_char - 'a';

	if (!t->roots[index]) {
		t->roots[index] = node_create(26);
	}
	trie_node_insert(t->roots[index], key + 1);
}

int trie_node_remove(struct trie_node **n, char *key)
{
	if (*key == '\0') {
		(*n)->words = 0;
		if (!(*n)->children_no) {
			trie_node_destroy(*n, 26);
			*n = NULL;
		}
		return 0;
	}

	size_t index = *key - 'a';
	if (!(*n)->children[index])
		return 1;

	int removal_status = trie_node_remove(&(*n)->children[index], key + 1);
	if (!(*n)->children[index]) {
		--(*n)->children_no;
		if (!(*n)->children_no && !(*n)->words) {
			trie_node_destroy(*n, 26);
			*n = NULL;
		}
	}

	return removal_status;
}

int trie_remove(struct trie *t, char *key)
{
	size_t index = *key - 'a';
	if (!t->roots[index])
		return 1; // TODO 1 = err code

	return trie_node_remove(&t->roots[index], key + 1);
}

void trie_node_print_all(struct trie_node *t)
{
	if (t->words) {
		printf(": %d\n", t->words);
	}
	for (size_t i = 0; i < 26; ++i) {
		if (t->children[i] != NULL) {
			printf("%c", (char)i + 'a');
			trie_node_print_all(t->children[i]);
		}
	}
}

void trie_print_all(struct trie *t)
{
	for (size_t i = 0; i < t->alphabet_size; ++i) {
		if (t->roots[i] != NULL) {
			printf("%c", (char)i + 'a');
			trie_node_print_all(t->roots[i]);
		}
	}
}