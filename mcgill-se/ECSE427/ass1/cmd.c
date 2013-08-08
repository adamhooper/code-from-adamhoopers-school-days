#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "cmd.h"

struct _Cmd
{
	char **argv;
	int background;
};

static char *
dupstrn (const char *s,
	 unsigned int n)
{
	char *r;

	r = malloc (sizeof (char) * (n + 1));

	if (r == NULL) return NULL;

	strncpy (r, s, n);

	*(r + n) = '\0';

	return r;
}

static char *
dupstr (const char *s)
{
	return dupstrn (s, strlen (s));
}

static char **
list_to_charpp (List *l)
{
	char **argv;
	char **a;
	unsigned int len;

	len = list_length (l);
	a = argv = malloc (sizeof (char *) * (len + 1));

	for (; l; l = l->next)
	{
		*a = dupstr ((char *) l->data);
		a++;
	}

	*a = NULL;

	return argv;
}

/*
 * Returns a newly-parsed Cmd, given a string of text input
 */
Cmd *
cmd_parse (const char *s)
{
	Cmd *cmd;
	List *list_argv;
	const char *start;
	int in_word = 0;
	int background;

	list_argv = NULL;

	for (start = s; *s; s++)
	{
		switch (*s)
		{
			case ' ':
			case '\t':
			case '\n': /* comes before '\0' */
				if (in_word)
				{
					in_word = 0;
					list_argv = list_prepend
						(list_argv,
						 dupstrn (start,
							  s - start));
				}
				start = s + 1;
				break;
			default:
				in_word = 1;
				break;
		}
	}

	/* Belongs in background? */
	if (list_argv
	    && list_argv->data
	    && *((char *) list_argv->data) == '&'
	    && *(((char *) list_argv->data) + 1) == '\0')
	{
		/* Remove "&" from args */
		free (list_argv->data);
		list_argv = list_remove (list_argv, list_argv->data);

		background = 1;
	}
	else
	{
		background = 0;
	}

	/* Empty command? */
	if (list_argv == NULL)
	{
		return NULL;
	}

	/* Create and return the Cmd* */
	list_argv = list_reverse (list_argv);

	cmd = malloc (sizeof (Cmd));
	cmd->background = background;
	cmd->argv = list_to_charpp (list_argv);

	list_foreach (list_argv, LIST_FUNC (free));
	list_free (list_argv);

	return cmd;
}

static char **
copy_strv (char * const * v)
{
	char * const * t;
	char **ret;
	char **t2;

	for (t = v; *t; t++);

	t2 = ret = malloc (sizeof (char *) * (t - v + 1));

	for (t = v; *t; t++)
	{
		*t2++ = dupstr (*t);
	}
	*t2 = NULL;

	return ret;
}

/*
 * Returns a deep copy of cmd
 */
Cmd *
cmd_copy (Cmd *cmd)
{
	Cmd *new;

	new = malloc (sizeof (Cmd));
	new->background = cmd->background;
	new->argv = copy_strv (cmd->argv);

	return new;
}

/*
 * Returns the argv array of cmd
 */
char * const *
cmd_get_argv (Cmd *cmd)
{
	return cmd->argv;
}

/*
 * Returns 1 if cmd should be started in the background, 0 otherwise
 */
int
cmd_get_background (Cmd *cmd)
{
	return cmd->background;
}

static void
free_argv (char **argv)
{
	char **a;

	for (a = argv; *a; a++)
	{
		free (*a);
	}

	free (argv);
}

/*
 * Frees cmd
 */
void
cmd_free (Cmd *cmd)
{
	if (cmd)
	{
		free_argv (cmd->argv);
		free (cmd);
	}
}

/*
 * Prints a description of cmd
 */
void
cmd_print (Cmd *cmd)
{
	char * const *a;
	int n;

	n = 0;
	for (a = cmd->argv; *a; a++)
	{
		if (n > 0)
		{
			printf (" ");
		}

		n++;
		printf ("%s", *a);
	}

	if (cmd->background == 1)
	{
		printf (" &");
	}

	printf ("\n");
}
