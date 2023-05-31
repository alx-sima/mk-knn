/* Copyright (C) 2023 Alexandru Sima (312CA) */

#include <stdio.h>
#include <string.h>

#include "mk_queries.h"
#include "trie_node.h"
#include "utils.h"

/**
 * @brief Functie de terminare a cautarii pentru primele 2 moduri de
 * autocomplete (s-a gasit primul cuvant).
 *
 * @param node nodul verificat
 * @retval 1   cautarea s-a terminat
 */
static int found_word(struct trie_node *node)
{
	return node->words;
}

/**
 * @brief Functie de terminare a cautarii pentru modul 3 de autocomplete (s-a
 * gasit cuvantul cu frecventa maxima).
 *
 * @param node  nodul verificat
 * @retval 1    cautarea s-a terminat
 */
static int found_frequent_prefix(struct trie_node *node)
{
	return node->words == node->max_freq;
}

/**
 * @brief Conditia pe care trebuie sa o indeplineasca un nod pentru a se avansa
 * in modul 1 de autocomplete (niciuna, trebuie doar sa existe).
 *
 * @param parent    nodul curent
 * @param child     nodul urmator candidat
 * @retval 1        avanseaza cautarea in nodul `child`.
 */
static int first_word_advance_condition(struct trie_node *parent,
										struct trie_node *child)
{
	/* Nodul urmator nu trebuie sa indeplineasca nicio conditie. */
	(void)parent, (void)child;
	return 1;
}

/**
 * @brief Conditia pe care trebuie sa o indeplineasca un nod pentru a se avansa
 * in modul 2 de autocomplete (sufixul minim al copilului trebuie sa fie cu 1
 * mai scurt decat cel curent; practic, sa conduca la acelasi cuvant).
 *
 * @param parent    nodul curent
 * @param child     nodul urmator candidat
 * @retval 1        avanseaza cautarea in nodul `child`.
 */
static int shortest_word_advance_condition(struct trie_node *parent,
										   struct trie_node *child)
{
	return child->min_word_len == parent->min_word_len - 1;
}

/**
 * @brief Conditia pe care trebuie sa o indeplineasca un nod pentru a se avansa
 * in modul 3 de autocomplete (copilul trebuie sa conduca catre un cuvant cu
 * aceeasi frecventa maxima).
 *
 * @param parent    nodul curent
 * @param child     nodul urmator candidat
 * @retval 1        avanseaza cautarea in nodul `child`.
 */
static int frequent_word_advance_condition(struct trie_node *parent,
										   struct trie_node *child)
{
	return child->max_freq == parent->max_freq;
}

/**
 * @brief intoarce sufixul cuvantului autocompletat conform functiilor date si
 * care incepe din node.
 *
 * @param node              nodul din care incepe sufixul
 * @param pos               pozitia lui `node` in stringul prefix
 * @param stop_condition    functie care determina cand se opreste cautarea
 * @param advance_condition functie care determina care nod este urmatorul
 * @return
 */
static char *autocomplete_prefix(struct trie_node *node, size_t pos,
								 int (*stop_condition)(struct trie_node *),
								 int (*advance_condition)(struct trie_node *,
														  struct trie_node *))
{
	if (stop_condition(node)) {
		char *word = malloc(pos + 1);
		DIE(!word, "failed malloc() of word buffer");

		word[pos] = '\0';
		return word;
	}

	for (size_t i = 0; i < 26; ++i) {
		if (!node->children[i])
			continue;

		if (advance_condition(node, node->children[i])) {
			char *word = autocomplete_prefix(node->children[i], pos + 1,
											 stop_condition, advance_condition);
			word[pos] = (char)i + 'a';
			return word;
		}
	}

	return NULL;
}

/**
 * @relates trie_node
 * @brief Construieste si afiseaza cuvantul care incepe cu prefix si are
 * proprietatile indicate de 2 functii date.
 *
 * @param node              nodul curent
 * @param prefix            prefixul pentru care se face autocomplete
 * @param stop_condition    functie care determina cand se opreste cautarea
 * @param advance_condition functie care determina care nod este urmatorul
 */
static void trie_node_autocomplete(struct trie_node *node, char *prefix,
								   int (*stop_condition)(struct trie_node *),
								   int (*advance_condition)(struct trie_node *,
															struct trie_node *))
{
	if (!node) {
		puts("No words found");
		return;
	}

	size_t prefix_len = strlen(prefix);
	char *word = autocomplete_prefix(node, prefix_len, stop_condition,
									 advance_condition);

	/* Adauga prefixul la inceputul cuvantului gasit. */
	strncpy(word, prefix, prefix_len);
	puts(word);

	free(word);
}

/**
 * @relates trie_node
 * @brief Formeaza toate cuvintele posibile cu aceeasi lungime cu `word` si
 * maximum `max_changes - changes` caractere diferite.
 *
 * @param node          nodul curent
 * @param word          cuvantul pana in acest moment
 * @param pos           pozitia la care se face schimbarea
 * @param changes       numarul de schimbari pana in acest moment
 * @param max_changes   numarul maxim de caractere care pot sa difere
 * @param alphabet_size numarul maxim de fii ai unui nod
 * @return              numarul de cuvinte gasite
 */
static int trie_node_autocorrect(struct trie_node *node, char *word, size_t pos,
								 int changes, int max_changes,
								 size_t alphabet_size)
{
	/* Daca s-a ajuns la finalul cuvantului si este un cuvant valid, se afiseaza
	 */
	if (word[pos] == '\0') {
		if (node->words) {
			puts(word);
			return 1;
		}
		return 0;
	}

	char oldchar = word[pos];
	int changes_total = 0;
	for (size_t i = 0; i < alphabet_size; ++i) {
		if (!node->children[i])
			continue;

		word[pos] = i + 'a';
		if (word[pos] == oldchar) {
			changes_total +=
				trie_node_autocorrect(node->children[i], word, pos + 1, changes,
									  max_changes, alphabet_size);
			continue;
		}

		/* Daca litera este diferita, se avanseaza in
		 * cautare doar daca se mai pot schimba caractere */
		if (changes == max_changes)
			continue;

		changes_total +=
			trie_node_autocorrect(node->children[i], word, pos + 1, changes + 1,
								  max_changes, alphabet_size);
	}

	word[pos] = oldchar;
	return changes_total;
}

void autocomplete(struct trie *dict, char *args)
{
	char *prefix = strtok(args, " ");
	char *arg = strtok(NULL, "");
	int type = strtol(arg, NULL, 0);

	static int (*const stop_conditions[])(struct trie_node *) = {
		found_word,
		found_word,
		found_frequent_prefix,
	};

	static int (*const advance_conditions[])(struct trie_node *,
											 struct trie_node *) = {
		first_word_advance_condition,
		shortest_word_advance_condition,
		frequent_word_advance_condition,
	};

	struct trie_node *start = trie_get_prefix(dict, prefix);

	if (type) {
		trie_node_autocomplete(start, prefix, stop_conditions[type - 1],
							   advance_conditions[type - 1]);
		return;
	}

	/* Pentru tipul 0 se afiseaza toate 3 completarile. */
	for (int i = 0; i < 3; ++i)
		trie_node_autocomplete(start, prefix, stop_conditions[i],
							   advance_conditions[i]);
}

void autocorrect(struct trie *dict, char *args)
{
	char *word = strtok(args, " ");
	char *arg = strtok(NULL, "");
	int chars = atoi(arg);
	if (!trie_node_autocorrect(dict->root, word, 0, 0, chars,
							   dict->alphabet_size))
		puts("No words found");
}
