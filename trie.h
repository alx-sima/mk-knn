/* Copyright 2023 Alexandru Sima (312CA) */
#ifndef TRIE_H_
#define TRIE_H_
#include <stddef.h>

struct trie_node {
	int words;
	struct trie_node **children;
	size_t children_no;
};

struct trie {
	struct trie_node **roots;
	size_t alphabet_size;
};

struct trie *trie_create(size_t alphabet_size);
void trie_destroy(struct trie *t);
int trie_remove(struct trie *t, char *key);

void trie_insert(struct trie *t, char *word);
void trie_print_all(struct trie *t);

#endif /* TRIE_H_ */