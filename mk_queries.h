/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef MK_QUERIES_H_
#define MK_QUERIES_H_

#include "trie.h"

/**
 * @relates trie
 * @brief Autocompleteaza prefixul dat.
 *
 * @param dict      dictionarul cu ajutorul caruia se face completarea
 * @param prefix    prefixul care se completeaza
 * @param type      tipul de autocomplete (0-3)
 */
void autocomplete(struct trie *dict, char *prefix, int type);

/**
 * @relates trie
 * @brief Afiseaza toate cuvintele care difera cu un maxim de `max_letters`
 * litere de original.
 *
 * @param dict          dictionarul care contine cuvintele
 * @param word          cuvantul original
 * @param max_letters   numarul maxim de litere care sa difere
 */
void autocorrect(struct trie *dict, char *word, int max_letters);

#endif /* MK_QUERIES_H_ */
