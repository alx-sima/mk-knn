/* Copyright (C) 2023 Alexandru Sima (312CA) */

#ifndef KBD_FUNCTIONS_H_
#define KBD_FUNCTIONS_H_

#include "trie.h"

void autocomplete(struct trie *dict, char *prefix, int type);

void autocorrect(struct trie *dict, char *prefix, int max_letters);

#endif /* KBD_FUNCTIONS_H_ */