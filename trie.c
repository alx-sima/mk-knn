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
	struct trie *trie = malloc(sizeof(struct trie));
	DIE(!trie, "failed malloc() of trie");

	trie->alphabet_size = alphabet_size;

	trie->root = trie_node_create(alphabet_size);

	return trie;
}

void trie_destroy(struct trie *trie)
{
	trie_node_destroy(trie->root, trie->alphabet_size);
	free(trie);
}

void trie_insert(struct trie *trie, char *word)
{
	size_t len_word = strlen(word);
	trie_node_insert(trie->root, word, len_word, trie->alphabet_size);
}

int trie_remove(struct trie *trie, char *word)
{
	return trie_node_remove(&trie->root, word, trie->alphabet_size);
}

struct trie_node *trie_get_prefix(struct trie *trie, char *prefix)
{
	return trie_node_get_prefix(trie->root, prefix);
}
