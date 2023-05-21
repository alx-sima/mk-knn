/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "trie_node.h"
#include "utils.h"

struct trie *trie_create(size_t alphabet_size)
{
	struct trie *t = malloc(sizeof(struct trie));
	DIE(!t, "failed malloc() of trie");

	t->alphabet_size = alphabet_size;

	t->root = node_create(alphabet_size);

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
