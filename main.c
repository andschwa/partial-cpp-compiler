/*
 * main.c - Accepts files for the 120++ lexical analyzer.
 *
 * Copyright (C) 2014 Andrew Schwartzmeyer
 *
 * This file released under the AGPLv3.
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "clex.h"
#include "token.h"
#include "list.h"
#include "cgram.tab.h"

struct token *yytoken = NULL;
struct list *tokens = NULL;
struct list *filenames = NULL;

/* multiple error labels were redundant when all we can do is exit */
void handle_error(char *c)
{
	perror(c);
	exit(EXIT_FAILURE);
}

char *current_filename()
{
	const char *filename = list_peek(filenames).filename;
	if (filename == NULL)
		return NULL;
	char *copy = calloc(strlen(filename)+1, sizeof(char));
	if (copy == NULL)
		handle_error("current_filename()");
	strcpy(copy, filename);
	return copy;
}

/* start the parser via yylex; error on relevant tokens; add good
   tokens to token list */
void parse_files()
{
	while (true) {
		int category = yylex();
		if (category == 0) {
			break;
		} else if (category < BEGTOKEN || category > ENDTOKEN ) {
			fprintf(stderr, "Unkown return value from yylex %d\n", category);
			exit(EXIT_FAILURE);
		}
		list_push(tokens, (union data)yytoken);
	}
	yylex_destroy();
	return;
}

int main(int argc, char **argv)
{
	tokens = list_init();
	filenames = list_init();
	if (tokens == NULL)
		handle_error("main tokens");
	if (filenames == NULL)
		handle_error("main filenames");

	if (argc == 1) {
		/* not 'char filename[] = "stdin"' because list_destroy */
		char *filename = malloc(sizeof("stdin"));
		if (filename == NULL)
			handle_error("main filename");
		strcpy(filename, "stdin");

		list_push(filenames, (union data)filename);
		yyin = stdin;
		yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
	} else {
		for (int i = 1; i < argc; ++i) {
			/* get real path for argument */
			char *filename = realpath(argv[i], NULL);
			if (filename == NULL)
				handle_error("main real path");

			/* open file and push filename and buffer */
			list_push(filenames, (union data)filename);
			yyin = fopen(filename, "r");
			if (yyin == NULL)
				handle_error("main file open");
			yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
		}
		/* start is a special case where we need to chdir to
		   the directory of the first file to be parsed; all
		   other files are chdir'ed to after the previous has
		   been popped */
		char *filename = current_filename();
		if (filename) {
			chdir(dirname(filename));
			free(filename);
		}
	}

	parse_files();

	printf("Line/Filename    Token   Text -> Ival/Sval\n");
	printf("------------------------------------------\n");
	const struct list_node *iter = list_head(tokens);
	while (!list_end(iter)) {
		char *filename = calloc(strlen(iter->data.token->filename)+1, sizeof(char));
		strcpy(filename, iter->data.token->filename);
		printf("%-5d%-12s%-8d%s ",
		       iter->data.token->lineno,
		       basename(filename),
		       iter->data.token->category,
		       iter->data.token->text);
		free(filename);

		if (iter->data.token->category == ICON)
			printf("-> %d", iter->data.token->ival);
		else if (iter->data.token->category == FCON)
			printf("-> %f", iter->data.token->fval);
		else if (iter->data.token->category == CCON)
			printf("-> %c", iter->data.token->ival);
		else if (iter->data.token->category == SCON)
			printf("-> %s", iter->data.token->sval);

		printf("\n");
		iter = iter->next;
	}

	list_destroy(tokens, (void (*)(union data))&token_free);
	list_destroy(filenames, (void (*)(union data))&free);

	return EXIT_SUCCESS;
}
