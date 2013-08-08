/*
 * main.c: main shell loop
 */

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "cmd.h"
#include "jobs.h"
#include "history.h"

#define BUFFER_SIZE 1024 /* FIXME: remove static-ness */
#define PROMPT " COMMAND-> "
#define PROMPT_LEN 11

static char *
read_line (const char *prompt)
{
	char *input = malloc (sizeof (char) * BUFFER_SIZE);
	ssize_t r;

	fprintf (stdout, "%s", prompt);
	fflush (stdout);

	for (;;)
	{
		r = read (STDIN_FILENO, input, BUFFER_SIZE - 1);

		if (r < 0)
		{
			if (errno == EINTR) /* Ctrl-C: prompt again */
			{
				continue;
			}

			if (errno == EIO)
			{
				fprintf (stderr, "Lost control of terminal\n");
				return NULL;
			}
		}

		if (r > 0)
		{
			char *t;

			*(input + BUFFER_SIZE - 1) = '\0';

			for (t = input; *t && *t != '\n'; t++);
			*(t + 1) = '\0';

			return input;
		}

		if (r == 0) /* EOF */
		{
			free (input);
			return NULL;
		}

		perror ("read");
		free (input);
		return NULL;
	}
}

static int
run_next_command (History *hist,
		  Jobs *jobs)
{
	char *line;
	Cmd *cmd;
	char * const * cmd_argv;
	int success;

	line = read_line (PROMPT);
	if (line == NULL) /* eof */
	{
		return 0;
	}

	cmd = cmd_parse (line);
	free (line);

	if (cmd == NULL)
	{
		return 1;
	}

	cmd_argv = cmd_get_argv (cmd);

	if (strcmp (*cmd_argv, "exit") == 0)
	{
		cmd_free (cmd);
		return 0;
	}

	if (strcmp (*cmd_argv, "r") == 0)
	{
		Cmd *hist_cmd;

		if (*(cmd_argv + 1) != NULL && *(cmd_argv + 2) != NULL)
		{
			fprintf (stderr, "r takes at most one argument\n");
			cmd_free (cmd);
			return 1;
		}

		hist_cmd = history_find_cmd (hist, *(cmd_argv + 1));

		if (hist_cmd == NULL)
		{
			if (*(cmd_argv + 1) == NULL)
			{
				fprintf (stderr, "History is empty\n");
			}
			else
			{
				fprintf (stderr,
					 "History command '%s' not found\n",
					 *(cmd_argv + 1));
			}
			cmd_free (cmd);
			return 1;
		}

		/* Replace cmd with the history one */
		cmd_free (cmd);
		cmd = hist_cmd;

		cmd_print (cmd);

		cmd_argv = cmd_get_argv (cmd);
	}

	if (strcmp (*cmd_argv, "cd") == 0)
	{
		success = builtin_cd (cmd_argv);
	}
	else if (strcmp (*cmd_argv, "fg") == 0)
	{
		success = builtin_fg (cmd_argv, jobs);
	}
	else if (strcmp (*cmd_argv, "jobs") == 0)
	{
		success = builtin_jobs (cmd_argv, jobs);
	}
	else if (strcmp (*cmd_argv, "help") == 0)
	{
		success = builtin_help (cmd_argv);
	}
	else if (strcmp (*cmd_argv, "history") == 0)
	{
		/*
		 * XXX: ugly way of getting "history" into history before we
		 * run it.
		 */
		if (*(cmd_argv + 1) == NULL)
		{
			history_append (hist, cmd);
		}
		builtin_history (cmd_argv, hist);
		success = -1;
	}
	else
	{
		jobs_add (jobs, cmd);
		success = 0;
	}

	if (success >= 0)
	{
		history_append (hist, cmd);
	}

	cmd_free (cmd);

	return 1;
}

static void
handle_sigint (int sig)
{
	/* Clearing of stdin is implicit in SIGINT */

	write (STDOUT_FILENO, "\n", 1);
	write (STDOUT_FILENO, PROMPT, PROMPT_LEN);

	signal (SIGINT, handle_sigint);
}

int
main (int argc,
      char **argv)
{
	History *hist;
	Jobs *jobs;

	if (isatty (STDIN_FILENO) == 0)
	{
		fprintf (stderr, "%s only supports an interactive console\n",
			 *argv);
		return 1;
	}

	signal (SIGINT, handle_sigint);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);

	hist = history_new ();
	jobs = jobs_new ();

	while (run_next_command (hist, jobs))
	{
		jobs_clean (jobs);
	}

	printf ("\n");

	jobs_free (jobs);
	history_free (hist);

	return 0;
}
