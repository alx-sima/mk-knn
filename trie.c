/* Copyright 2023 Alexandru Sima (312CA) */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "trie.h"

struct node *node_create(size_t alphabet_size)
{
	struct node *n = malloc(sizeof(struct node));
	if (!n)
		return NULL;

	n->children = calloc(alphabet_size, sizeof(struct node *));
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

	t->roots = calloc(alphabet_size, sizeof(struct node *));
	if (!t->roots) {
		free(t);
		return NULL;
	}

	t->alphabet_size = alphabet_size;
	return t;
}

void node_destroy(struct node *n, size_t alphabet_size)
{
	for (size_t i = 0; i < alphabet_size; ++i) {
		if (n->children[i])
			node_destroy(n->children[i], alphabet_size);
	}

	free(n->children);
	free(n);
}

void trie_destroy(struct trie *t)
{
	for (size_t i = 0; i < t->alphabet_size; ++i) {
		if (t->roots[i])
			node_destroy(t->roots[i], t->alphabet_size);
	}

	free(t->roots);
	free(t);
}

void node_insert(struct node *t, char *word)
{
	if (*word == '\0') {
		++t->words;
		return;
	}
	size_t char_index = *word - 'a';
	if (t->children[char_index] == NULL) {
		++t->children_no;
		t->children[char_index] = node_create(26);
	}
	// TODO programare defensiva
	node_insert(t->children[char_index], word + 1);
}

void trie_insert(struct trie *t, char *word)
{
	char current_char = *word;
	size_t index = current_char - 'a';

	if (!t->roots[index]) {
		t->roots[index] = node_create(26);
	}
	node_insert(t->roots[index], word + 1);
}

int node_remove(struct node **n, char *word)
{
	if (*word == '\0') {
		(*n)->words = 0;
		if (!(*n)->children_no) {
			node_destroy(*n, 26);
			*n = NULL;
		}
		return 0;
	}

	size_t index = *word - 'a';
	if (!(*n)->children[index])
		return 1;

	int removal_status = node_remove(&(*n)->children[index], word + 1);
	if (!(*n)->children[index]) {
		--(*n)->children_no;
		if (!(*n)->children_no && !(*n)->words) {
			node_destroy(*n, 26);
			*n = NULL;
		}
	}

	return removal_status;
}

int trie_remove(struct trie *t, char *word)
{
	size_t index = *word - 'a';
	if (!t->roots[index])
		return 1; // TODO 1 = err code

	return node_remove(&t->roots[index], word + 1);
}

void node_print_all(struct node *t)
{
	if (t->words) {
		printf(": %d\n", t->words);
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
	for (size_t i = 0; i < t->alphabet_size; ++i) {
		if (t->roots[i] != NULL) {
			printf("%c", (char)i + 'a');
			node_print_all(t->roots[i]);
		}
	}
}