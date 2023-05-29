### Copyright (C) 2023 Alexandru Sima (312CA) ###

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g -MMD -MP

DEPS=$(wildcard *.d)

# define targets
TARGETS=kNN mk

build: $(TARGETS)

mk: mk.o mk_functions.o trie.o trie_node.o io.o
	$(CC) $(CFLAGS) $^ -o $@

kNN: kNN.o array.o bst.o io.o point.o
	$(CC) $(CFLAGS) $^ -o $@

#%.o: %.c
#	$(CC) $(CFLAGS) $^ -o $@ -c

-include $(DEPS)

pack:
	zip -FSr 312CA_AlexandruSima_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) *.d *.o vgcore.*

.PHONY: pack clean
