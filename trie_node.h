/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef TRIE_NODE_H_
#define TRIE_NODE_H_

#include <stddef.h>

/**
 * @class trie_node
 * @brief Un nod dintr-un trie.
 */
struct trie_node {
	/** Fiii nodului */
	struct trie_node **children;

	/** Numarul de cuvinte care incep cu
	 * acest prefix (numarul de fii) */
	size_t children_no;

	/** Numarul de cuvinte memorate care se termina
	 * in stringul format pana in acest nod */
	size_t words;

	/** Lungimea minima a unui sufix care incepe din
	 * nodul curent si duce la un cuvant valid. */
	size_t min_word_len;
	/** Frecventa maxima a unui cuvant care se
	 * ramifica din nodul curent .*/
	size_t max_freq;
};

/**
 * @relates trie_node
 * @brief Creaza un nou nod de trie.
 *
 * @param alphabet_size numarul maxim posibil de fii
 * @return              nodul creat
 */
struct trie_node *trie_node_create(size_t alphabet_size);

/**
 * @relates trie_node
 * @brief Sterge un nod de trie.
 *
 * @param node          nodul de sters
 * @param alphabet_size numarul maxim posibil de fii ai nodului
 */
void trie_node_destroy(struct trie_node *node, size_t alphabet_size);

/**
 * @relates trie_node
 * @brief Insereaza un cuvant (sau un sufix al acestuia),
 * incepand din nodul dat.
 *
 * @param node          nodul in care se insereaza
 * @param word          sufixul care se insereaza
 * @param word_len      lungimea sufixului
 * @param alphabet_size numarul maxim de fii ai unui nod
 */
void trie_node_insert(struct trie_node *node, char *word, size_t word_len,
					  size_t alphabet_size);

/**
 * @relates trie_node
 * @brief Sterge un cuvant (sau un sufix al acestuia),
 * incepand din nodul dat.
 *
 * @param node          nodul din care se sterge
 * @param word          cuvantul de sters
 * @param alphabet_size numarul maxim de fii ai unui nod
 * @return
 */
int trie_node_remove(struct trie_node **node, char *word, size_t alphabet_size);

/**
 * @relates trie_node
 * @brief Intoarce nodul corespunzator prefixului dat.
 *
 * @param node      nodul din care incepe cautarea
 * @param prefix    prefixul cautat
 * @return
 */
struct trie_node *trie_node_get_prefix(struct trie_node *node, char *prefix);

#endif /* TRIE_NODE_H_ */
