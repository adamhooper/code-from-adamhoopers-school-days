#ifdef DEBUG
#include <assert.h>
#endif /* DEBUG */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"
#include "list.h"

#define MAX_ELEMS 10

struct _History
{
	List *cmds; /* Queue of Cmd*'s */
	unsigned int num;
	unsigned int list_size;
};

/*
 * Returns a new History object
 */
History *
history_new (void)
{
	History *ret;

	ret = calloc (1, sizeof (History));

	return ret;
}

/*
 * Appends a command to the history
 */
void
history_append (History *h,
		Cmd *cmd)
{
	h->num++;

	if (h->list_size >= MAX_ELEMS)
	{
		/* Remove the tail of the queue */
		List *l;

#ifdef DEBUG
		assert (h->cmds != NULL && h->cmds->next != NULL);
#endif /* DEBUG */

		for (l = h->cmds; l->next->next; l = l->next);

		cmd_free ((Cmd *) l->next->data);
		list_free (l->next);
		l->next = NULL;
	}
	else
	{
		h->list_size++;
	}

	h->cmds = list_prepend (h->cmds, cmd_copy (cmd));
}

/*
 * Searches the history for the Cmd starting with (possibly-NULL) argv0
 *
 * Returns a copy of the Cmd
 */
Cmd *
history_find_cmd (History *h,
		  const char *argv0)
{
	List *l;
	Cmd *cmd;
	char * const *cmd_argv;

	if (argv0 == NULL)
	{
		if (h->cmds == NULL)
		{
			return NULL;
		}
		else
		{
			return cmd_copy ((Cmd *) h->cmds->data);
		}
	}
	else
	{
		for (l = h->cmds; l; l = l->next)
		{
			cmd = (Cmd *) l->data;

			cmd_argv = cmd_get_argv (cmd);
			if (strncmp (*cmd_argv, argv0, strlen (argv0)) == 0)
			{
				return cmd_copy (cmd);
			}
		}
	}

	return NULL;
}

/*
 * Frees h
 */
void
history_free (History *h)
{
	if (h != NULL)
	{
		List *l;

		for (l = h->cmds; l; l = l->next)
		{
			cmd_free ((Cmd *) l->data);
		}

		list_free (h->cmds);

		free (h);
	}
}

/*
 * Prints the most recently appended items in h
 */
void
history_print (History *h)
{
	List *l;
	int i;

	printf ("History of %d (%d) elements:\n", h->num, h->list_size);

	i = h->num;
	for (l = h->cmds; l; l = l->next)
	{
		printf ("%d: ", i);
		cmd_print ((Cmd *) l->data);
		i--;
	}
}
