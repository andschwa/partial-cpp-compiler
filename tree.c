/*
 * tree.c - Implementation of tree.
 *
 * Copyright (C) 2014 Andrew Schwartzmeyer
 *
 * This file released under the AGPLv3.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "tree.h"
#include "list.h"

bool tree_default_compare(void *a, void *b);

/*
 * Initializes tree with reference to parent and data, and an empty
 * (but initialized) list of children.
 */
struct tree *tree_new(struct tree *parent, void *data,
                      bool (*compare)(void *a, void *b),
                      void (*delete)(void *data, bool leaf))
{
	struct tree *t = malloc(sizeof(*t));
	if (t == NULL) {
		perror("tree_new()");
		return NULL;
	}

	struct list *l = list_new(compare, NULL);
	if (l == NULL) {
		free(t);
		return NULL;
	}

	t->parent = parent;
	t->data = data;
	t->children = l;
	t->compare = (compare == NULL)
		? &tree_default_compare
		: compare;
	t->delete = delete;

	return t;
}

/*
 * Initializes tree with reference to parent and data, and pushes
 * count number of following struct tree * as children. If given child
 * is NULL it is not added.
 */
struct tree *tree_new_group(struct tree *parent, void *data,
                            bool (*compare)(void *a, void *b),
                            void (*delete)(void *data, bool leaf),
                            int count, ...)
{
	va_list ap;
	va_start(ap, count);

	struct tree *t = tree_new(parent, data, compare, delete);

	for (int i = 0; i < count; ++i)
		tree_push_child(t, va_arg(ap, void *));

	va_end(ap);
	return t;
}

/*
 * Recursively calculates size of tree.
 */
size_t tree_size(struct tree *self)
{
	if (self == NULL) {
		fprintf(stderr, "tree_size(): self was null\n");
		return 0;
	}

	size_t size = 1;

	struct list_node *iter = list_head(self->children);
	while (!list_end(iter)) {
		size += tree_size(iter->data);
		iter = iter->next;
	}

	return size;
}

/*
 * Pre-order traversal of tree. Takes a function and applies it to
 * each subtree.
 */
void tree_preorder(struct tree *self, int d, bool (*f)(struct tree *t, int d))
{
	if (self == NULL) {
		fprintf(stderr, "tree_preorder(): self was null\n");
		return;
	}

	bool recurse = true;
	if (f != NULL)
		recurse = f(self, d);

	if (recurse) {
		struct list_node *iter = list_head(self->children);
		while (!list_end(iter)) {
			tree_preorder(iter->data, d+1, f);
			iter = iter->next;
		}
	}
}

/*
 * Initializes a child tree with self as parent and data reference.
 */
struct tree *tree_leaf(struct tree *self, void *data)
{
	if (self == NULL) {
		fprintf(stderr, "tree_leaf(): self was null\n");
		return NULL;
	}

	struct tree *child = tree_new(self, data, self->compare, self->delete);
	if (child == NULL) {
		perror("tree_leaf()");
		return NULL;
	}

	return child;
}

/*
 * Pushes child data to front. Returns reference to child leaf.
 */
struct tree *tree_push_front(struct tree *self, void *data)
{

	struct tree *child = tree_leaf(self, data);

	list_push_front(self->children, child);

	return child;
}

/*
 * Pushes child data to back. Returns reference to child leaf.
 */
struct tree *tree_push_back(struct tree *self, void *data)
{
	struct tree *child = tree_leaf(self, data);

	list_push_back(self->children, child);

	return child;
}

/*
 * Given an initialized child, pushes it to back of the children list.
 */
struct tree *tree_push_child(struct tree *self, struct tree *child)
{
	if (self == NULL) {
		fprintf(stderr, "tree_push_child(): self was null\n");
		return NULL;
	}

	if (child == NULL) /* not an error */
		return NULL;

	child->parent = self;
	list_push_back(self->children, child);

	return child;
}

/*
 * Return the subtree at pos in the children list of self.
 */
struct tree *tree_index(struct tree *self, int pos)
{
	struct list_node *n = list_index(self->children, pos);
	if (!list_end(n))
		return n->data;
	return NULL;
}

/*
 * Recursively deallocates a tree, and optionally frees data if given
 * a non-NULL function pointer, which accepts a pointer to data and a
 * boolean that indicates whether or not the data came from a leaf
 * node.
 */
void tree_free(struct tree *self)
{
	if (self == NULL) {
		fprintf(stderr, "tree_free(): self was null\n");
		return;
	}

	if (self->delete)
		self->delete(self->data, list_empty(self->children));

	while (!list_empty(self->children)) {
		void *d = list_pop_back(self->children);
		tree_free(d);
	}

	free(self->children->sentinel);
	free(self->children);
}

/*
 * Default comparison for string keys.
 */
bool tree_default_compare(void *a, void *b)
{
	return (strcmp((char *)a, (char *)b) == 0);
}
