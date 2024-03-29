### Copyright (C) 2023 Alexandru Sima (312CA) ###

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g -MMD -MP

DEPS=$(wildcard *.d)

# define targets
TARGETS=kNN mk

build: $(TARGETS)

mk: mk.o mk_queries.o trie.o trie_node.o utils.o
	$(CC) $(CFLAGS) $^ -o $@

kNN: kNN.o kNN_queries.o array.o bst.o point.o utils.o
	$(CC) $(CFLAGS) $^ -o $@

-include $(DEPS)

doc:
	doxygen

pack:
	zip -FSr 312CA_AlexandruSima_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) *.d *.o vgcore.*

.PHONY: pack clean
