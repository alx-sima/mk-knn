# Copyright 2023 Alexandru Sima (312CA)
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=kNN mk

build: $(TARGETS)

mk: mk.o trie.o trie_internals.o io.o
	$(CC) $(CFLAGS) $^ -o $@

kNN: kNN.o bst.o io.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@ -c -MMD -MP -MF $(@:.o=.d)

-include $(wilcard *.d)

pack:
	zip -FSr 312CA_AlexandruSima_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) *.d *.o vgcore.*

.PHONY: pack clean
