/* Copyright 2023 Alexandru Sima */
#ifndef TRIE_INTERNALS_H_
#define TRIE_INTERNALS_H_
#include <stddef.h>

/**
 * @class node
 * @brief Un nod dintr-un trie.
 */
struct node {
	/** Fiii nodului */
	struct node **children;

	/** Numarul de cuvinte care incep cu
	 * acest prefix (numarul de fii) */
	size_t children_no;

	/** Numarul de cuvinte memorate care se termina
	 * in stringul format pana in acest nod */
	size_t words;

	size_t min_word_len;
	size_t max_freq;
};

struct node *node_create(size_t alphabet_size);

void node_destroy(struct node *n, size_t alphabet_size);

void node_insert(struct node *n, char *word, size_t word_len);

int node_remove(struct node **n, char *word);

#endif /* TRIE_INTERNALS_H_ */