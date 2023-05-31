/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "trie_node.h"
#include "utils.h"

/**
 * @relates trie_node
 * @brief Recalculeaza proprietatile `min_word_len` si `max_freq` ale unui nod.
 *
 * @param node          nodul dat
 * @param alphabet_size numarul maxim de fii ai nodului
 */
static void trie_node_recalculate_properties(struct trie_node *node,
											 size_t alphabet_size)
{
	/* Daca nodul curent marcheaza finalul unui cuvant,
	 * lungimea minima a sufixului este, evident, 0.
	 * In caz contrar, lungimea initiala este "+INF".  */
	node->min_word_len = node->words ? 0 : ULONG_MAX;
	node->max_freq = node->words;

	for (size_t i = 0; i < alphabet_size; ++i) {
		if (!node->children[i])
			continue;

		if (node->children[i]->min_word_len < node->min_word_len)
			node->min_word_len = node->children[i]->min_word_len + 1;

		if (node->children[i]->max_freq > node->max_freq)
			node->max_freq = node->children[i]->max_freq;
	}
}

struct trie_node *trie_node_create(size_t alphabet_size)
{
	struct trie_node *node = malloc(sizeof(struct trie_node));
	DIE(!node, "failed malloc() of trie_node");

	node->children = calloc(alphabet_size, sizeof(struct trie_node *));
	DIE(!node->children, "failed malloc() of trie_node->children");

	node->children_no = 0;
	node->min_word_len = 0;
	node->words = 0;
	return node;
}

void trie_node_destroy(struct trie_node *node, size_t alphabet_size)
{
	for (size_t i = 0; i < alphabet_size; ++i) {
		if (node->children[i])
			trie_node_destroy(node->children[i], alphabet_size);
	}

	free(node->children);
	free(node);
}

void trie_node_insert(struct trie_node *node, char *word, size_t word_len,
					  size_t alphabet_size)
{
	if (word_len == 0) {
		++node->words;
		trie_node_recalculate_properties(node, alphabet_size);
		return;
	}

	size_t char_index = *word - 'a';
	/* Se creaza nodul urmator daca nu exista */
	if (!node->children[char_index]) {
		node->children[char_index] = trie_node_create(alphabet_size);
		++node->children_no;
	}

	trie_node_insert(node->children[char_index], word + 1, word_len - 1,
					 alphabet_size);
	trie_node_recalculate_properties(node, alphabet_size);
}

int trie_node_remove(struct trie_node **node, char *word, size_t alphabet_size)
{
	if (*word == '\0') {
		(*node)->words = 0;
		if (!(*node)->children_no) {
			trie_node_destroy(*node, alphabet_size);
			*node = NULL;
		}
		return 0;
	}

	size_t index = *word - 'a';
	if (!(*node)->children[index])
		return 1;

	int removal_status =
		trie_node_remove(&(*node)->children[index], word + 1, alphabet_size);
	if (!(*node)->children[index]) {
		--(*node)->children_no;

		if (!(*node)->children_no && !(*node)->words) {
			trie_node_destroy(*node, alphabet_size);
			*node = NULL;
			return 0;
		}
	}

	trie_node_recalculate_properties(*node, alphabet_size);
	return removal_status;
}

struct trie_node *trie_node_get_prefix(struct trie_node *node, char *prefix)
{
	if (*prefix == '\0')
		return node;

	size_t index = *prefix - 'a';
	if (!node->children[index])
		return NULL;

	return trie_node_get_prefix(node->children[index], prefix + 1);
}
