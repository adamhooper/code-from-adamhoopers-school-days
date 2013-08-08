#ifndef _LIST_H
#define _LIST_H

/*
 * Simple singly-linked list implementation.
 *
 * This was modeled after GLib's GSList, but no code was copied. After all,
 * it's not hard to code a linked list.
 */

typedef struct _List List;

struct _List {
	void *data;
	List *next;
};

typedef void (*ListFunc) (void *data);
#define LIST_FUNC(f) ((ListFunc) (f))

List		*list_prepend 	(List *list, void *data);
List		*list_append 	(List *list, void *data);
List		*list_reverse 	(List *list);
unsigned int	 list_length	(List *list);
List		*list_remove 	(List *list, const void *data);
void		 list_free 	(List *list);
void		 list_foreach 	(List *list, ListFunc fn);

#endif /* _LIST_H */
