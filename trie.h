/* Copyright 2023 Alexandru Sima (312CA) */
#ifndef TRIE_H_
#define TRIE_H_
#include <stddef.h>

struct node;

/**
 * @class trie
 * @brief Structura care retine cuvinte si aparitiile
 * lor, folosind arbori de prefixe.
 */
struct trie {
	/** `alphabet_size` noduri care reprezinta cate
	 * un arbore pentru fiecare litera a alfabetului */
	struct node *root;
	/** Numarul maxim de fii al unui nod */
	size_t alphabet_size;
};

/**
 * @relates trie
 * @brief Instantiaza un nou trie.
 *
 * @param alphabet_size numarul de fii pe care ii are fiecare nod
 * @return structura alocata
 * @retval NULL eroare de alocare
 */
struct trie *trie_create(size_t alphabet_size);

/**
 * @relates trie
 * @brief Dealoca memoria unui trie.
 *
 * @param t trie-ul dealocat
 */
void trie_destroy(struct trie *t);

/**
 * @relates trie
 * @brief Sterge toate aparitiile cuvantului `word` din trie.
 *
 * @param t		trie-ul in care se efectueaza stergerea
 * @param word	cuvantul sters
 *
 * @retval 0 cuvantul a fost sters
 * @retval 1 cuvantul nu exista
 */
int trie_remove(struct trie *t, char *word);

/**
 * @relates trie
 * @brief Insereaza cuvantul `word` in trie, sau incrementeaza
 * numarul de aparitii daca exista deja.
 *
 * @param t		trie-ul in care se insereaza
 * @param word	cuvantul de inserat
 */
void trie_insert(struct trie *t, char *word);

struct node *trie_get_prefix(struct trie *t, char *prefix);

#endif /* TRIE_H_ */