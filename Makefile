# Makfile - Creates the 120++ compiler.
# Copyright (C) 2014 Andrew Schwartzmeyer
# This file released under the AGPLv3.

SHELL = /bin/sh

# generated executables
BIN = 120
TESTS = test-list test-tree test-hasht

# dependencies
CC = gcc
FLEX = flex
BISON = bison

# flags
CDEBUG = -g
CFLAGS = -O -Wall -Werror -std=gnu99
LDFLAGS = -g
FLEXFLAGS =
BISONFLAGS = -Wall -Werror

# git archive option
GITREF = $(shell git tag | tail -n 1)

# local machine specifics
-include local.mk

# files
SRCS = main.c logger.c symbol.c node.c token.c rules.c scope.c intermediate.c \
	list.c tree.c hasht.c lookup3.c \
	lex.yy.c parser.tab.c
OBJS = $(SRCS:.c=.o)
TEST_DATA = data/pass/test.cpp

# targets
.PHONY: all test smoke dist clean distclean

all: $(BIN)

test: $(TESTS)
	./test-list
	./test-tree
	./test-hasht

smoke: test all
	./$(BIN) $(TEST_DATA)

TAGS: $(SRCS)
	etags $(SRCS)
dist:
	git archive --format=tar $(GITREF) > $(GITREF).tar

clean:
	rm -f $(BIN) $(TESTS) *.o lexer.h lex.yy.c parser.tab.h parser.tab.c

distclean: clean
	rm -f TAGS *.tar

# sources
$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(CDEBUG) -o $@ -c $<

main.o: args.h logger.h libs.h lexer.h symbol.h node.h intermediate.h list.h tree.h hasht.h

logger.o: logger.h args.h node.h token.h lexer.h symbol.h list.h tree.h

lexer.h: lex.yy.c

lex.yy.c: lexer.l
	$(FLEX) $(FLEXFLAGS) $<

lexer.l: args.h node.h logger.h token.h libs.h parser.tab.h rules.h \
	list.h tree.h hasht.h

parser.tab.h parser.tab.c: parser.y
	$(BISON) $(BISONFLAGS) $<

parser.y: node.h logger.h token.h rules.h list.h tree.h

symbol.o: symbol.h args.h logger.h node.h token.h libs.h \
	rules.h scope.h lexer.h parser.tab.h list.h hasht.h tree.h

node.o: node.h logger.h tree.h rules.h

token.o: token.h logger.h parser.tab.h

scope.o: scope.h symbol.h list.h hasht.h

intermediate.o: intermediate.h symbol.h logger.h node.h list.h tree.h

list.o: list.h

tree.o: tree.h list.h

hasht.o: hasht.h lookup3.o

test.o: test.h

BUILD_TEST = $(CC) $(CFLAGS) $(CDEBUG) -o $@ $^
test-list: test_list.o list.o test.o
	$(BUILD_TEST)

test-tree: test_tree.o tree.o list.o test.o
	$(BUILD_TEST)

test-hasht: test_hasht.o hasht.o lookup3.o test.o
	$(BUILD_TEST)
