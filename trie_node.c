/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "trie_node.h"
#include "utils.h"

struct node *node_create(size_t alphabet_size)
{
	struct node *n = malloc(sizeof(struct node));
	DIE(!n, "failed malloc() of node");

	n->children = calloc(alphabet_size, sizeof(struct node *));
	DIE(!n->children, "failed malloc() of node->children");

	n->children_no = 0;
	n->min_word_len = 0;
	n->words = 0;
	return n;
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

void node_insert(struct node *n, char *word, size_t word_len)
{
	if (word_len == 0) {
		++n->words;
		n->max_freq = n->words;

		n->min_word_len = 0;
		for (size_t i = 0; i < 26; ++i) {
			if (!n->children[i])
				continue;

			if (n->children[i]->max_freq > n->max_freq)
				n->max_freq = n->children[i]->max_freq;
		}
		return;
	}

	size_t char_index = *word - 'a';
	if (!n->children[char_index]) {
		n->children[char_index] = node_create(26);
		++n->children_no;
	}

	node_insert(n->children[char_index], word + 1, word_len - 1);

	n->min_word_len = ULONG_MAX;
	if (n->words)
		n->min_word_len = 0;
	n->max_freq = n->words;
	for (size_t i = 0; i < 26; ++i) {
		if (!n->children[i])
			continue;

		if (n->children[i]->min_word_len < n->min_word_len)
			n->min_word_len = n->children[i]->min_word_len + 1;

		if (n->children[i]->max_freq > n->max_freq)
			n->max_freq = n->children[i]->max_freq;
	}
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
			return 0;
		}
	}

	(*n)->min_word_len = ULONG_MAX;
	if ((*n)->words)
		(*n)->min_word_len = 0;
	(*n)->max_freq = (*n)->words;
	for (size_t i = 0; i < 26; ++i) {
		if (!(*n)->children[i])
			continue;

		if ((*n)->children[i]->min_word_len < (*n)->min_word_len)
			(*n)->min_word_len = (*n)->children[i]->min_word_len + 1;

		if ((*n)->children[i]->max_freq > (*n)->max_freq)
			(*n)->max_freq = (*n)->children[i]->max_freq;
	}

	return removal_status;
}
