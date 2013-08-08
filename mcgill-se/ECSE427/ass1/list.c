#include <stdlib.h>

#include "list.h"

static List *
list_alloc (void)
{
	return (List *) malloc (sizeof (List));
}

List *
list_prepend (List *list,
	      void *data)
{
	List *ret = list_alloc ();
	ret->data = data;
	ret->next = list;
	return ret;
}

List *
list_append (List *list,
	     void *data)
{
	List *ret;

	if (list == NULL) return list_prepend (list, data);

	ret = list;

	for (; list->next; list = list->next);
	list->next = list_alloc ();
	list->next->data = data;
	list->next->next = NULL;

	return ret;
}

List *
list_reverse (List *list)
{
	List *prev = NULL;
	List *next;

	while (list)
	{
		next = list->next;

		list->next = prev;

		prev = list;
		list = next;
	}

	return prev;
}

List *
list_remove (List *list,
	     const void *data)
{
	List *prev = NULL;
	List *ret = list;

	while (list != NULL)
	{
		if (list->data == data)
		{
			if (prev == NULL)
			{
				ret = list->next;
				free (list);
				return ret;
			}
			else
			{
				prev->next = list->next;
				free (list);
				return ret;
			}
		}

		prev = list;
		list = list->next;
	}

	return ret;
}

unsigned int
list_length (List *list)
{
	unsigned int ret = 0;

	while (list)
	{
		ret++;
		list = list->next;
	}

	return ret;
}

void
list_free (List *list)
{
	List *next;

	while (list)
	{
		next = list->next;
		free (list);
		list = next;
	}
}

void
list_foreach (List *list,
	      ListFunc fn)
{
	for (; list; list = list->next)
	{
		(*fn) (list->data);
	}
}
