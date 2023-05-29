/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef MK_FUNCTIONS_H_
#define MK_FUNCTIONS_H_

#include "trie.h"

void autocomplete(struct trie *dict, char *prefix, int type);

void autocorrect(struct trie *dict, char *prefix, int max_letters);

#endif /* MK_FUNCTIONS_H_ */
