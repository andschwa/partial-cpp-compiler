/*
 * final.c - Implementation of final code generation.
 *
 * Copyright (C) 2014 Andrew Schwartzmeyer
 *
 * This file released under the AGPLv3.
 *
 */

#include <stdio.h>

#include "type.h"

#include "list.h"
#include "hasht.h"

extern struct list *yyscopes;

static char *map_region(enum region r);
static void map_address(FILE *stream, struct address a);

void final_code(FILE *stream, struct list *code)
{
	fprintf(stream, "int main()\n{\n");
	fprintf(stream, "\t/* initializing constant region */\n");
	struct hasht *constant = list_front(yyscopes);
	for (size_t i = 0; i < constant->size; ++i) {
		struct hasht_node *slot = constant->table[i];
		if (slot && !hasht_node_deleted(slot)) {
			struct typeinfo *v = slot->value;
			if (v->base == FLOAT_T || (v->base == CHAR_T && v->pointer)) {
				fprintf(stream, "\t");
				map_address(stream, v->place);
				fprintf(stream, " = %s;\n", slot->key);
			}
		}
	}
	fprintf(stream, "}");

}

static void map_address(FILE *stream, struct address a)
{
	fprintf(stream, "(*(%s %s*)(%s + %d))",
	        print_basetype(a.type), a.type->pointer ? "*" : "",
	        map_region(a.region), a.offset);
}

static char *map_region(enum region r)
{
	switch (r) {
	case GLOBE_R:
		return "global";
	case CONST_R:
		return "constant";
	default:
		return "unimplemented";
	}
}
