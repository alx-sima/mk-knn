/* Copyright 2023 Alexandru Sima (312CA) */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	n->min_word_len = 0;
	n->words = 0;
	return n;
}

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
	node_destroy(t->root, t->alphabet_size);
	free(t);
}

void node_insert(struct node *n, char *word, size_t word_len)
{
	if (word_len == 0) {
		++n->words;
		n->max_freq = n->words; // TODO
		return;
	}

	size_t char_index = *word - 'a';
	if (n->children[char_index] == NULL) {
		n->children[char_index] = node_create(26);
		++n->children_no;
	}
	// TODO programare defensiva
	node_insert(n->children[char_index], word + 1, word_len - 1);

	n->min_word_len = 0xfffffff;
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

void trie_insert(struct trie *t, char *word)
{
	size_t len_word = strlen(word);
	node_insert(t->root, word, len_word);
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

		for (size_t i = 0; i < 26; ++i) {
			if (!(*n)->children[i])
				continue;

			if ((*n)->children[i]->min_word_len < (*n)->min_word_len)
				(*n)->min_word_len = (*n)->children[i]->min_word_len + 1;
		}
	}

	// TODO
	return removal_status;
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

void get_first_word(struct node *n, char *resultstr)
{
	for (size_t i = 0; i < 26; ++i) {
		if (n->children[i]) {
			*resultstr = (char)i + 'a';
			get_first_word(n->children[i], resultstr + 1);
			return;
		}
	}

	*resultstr = '\0';
}

int get_shortest_word(struct node *n, char *resultstr, size_t pos)
{
	if (n->words) {
		resultstr[pos] = '\0';
		return 0;
	}

	// if (n->min_word_len == 0)
	//	return 1;

	for (size_t i = 0; i < 26; ++i) {
		if (!n->children[i])
			continue;

		if (n->children[i]->min_word_len == n->min_word_len - 1) {
			int status = get_shortest_word(n->children[i], resultstr, pos + 1);
			resultstr[pos] = i + 'a';
			return status;
		}
	}

	return 1;
}

void get_most_frequent_word(struct node *n, char *resultstr, size_t pos)
{
	if (n->words == n->max_freq) {
		resultstr[pos] = '\0';
		return;
	}

	for (size_t i = 0; i < 26; ++i) {
		if (!n->children[i])
			continue;

		if (n->children[i]->max_freq == n->max_freq) {
			get_most_frequent_word(n->children[i], resultstr, pos + 1);
			resultstr[pos] = i + 'a';
			return;
		}
	}
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
	node_print_all(t->root);
}