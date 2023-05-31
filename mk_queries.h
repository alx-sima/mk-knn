/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef MK_QUERIES_H_
#define MK_QUERIES_H_

#include "trie.h"

/**
 * @relates trie
 * @brief Autocompleteaza prefixul dat.
 *
 * @param dict      dictionarul cu ajutorul caruia se face completarea
 * @param args      argumentele comenzii, sub forma de string
 */
void autocomplete(struct trie *dict, char *args);

/**
 * @relates trie
 * @brief Afiseaza toate cuvintele care difera cu un maxim de `max_letters`
 * litere de original.
 *
 * @param dict          dictionarul care contine cuvintele
 * @param args          argumentele comenzii, sub forma de string
 */
void autocorrect(struct trie *dict, char *args);

#endif /* MK_QUERIES_H_ */
