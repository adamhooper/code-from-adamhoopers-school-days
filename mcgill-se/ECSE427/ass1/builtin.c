#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "builtin.h"

/*
 * Changes to the given directory (or $HOME if no directory is given)
 */
int
builtin_cd (char * const *argv)
{
	const char *dest;

	if (*(argv + 1) == NULL)
	{
		if (getenv ("HOME") == NULL)
		{
			fprintf (stderr,
				 "Could not determine home directory\n");
			return -1;
		}

		dest = getenv ("HOME");
	}
	else
	{
		dest = *(argv + 1);
	}

	if (*(argv + 2) != NULL)
	{
		fprintf (stderr, "Invalid arguments to cd\n");
		return -1;
	}

	if (chdir (dest) != 0)
	{
		perror ("chdir");
		return errno;
	}

	return 0;
}

/*
 * Brings the specified (or implicit) job to the foreground
 */
int
builtin_fg (char * const *argv,
	    Jobs *jobs)
{
	if (*(argv + 1) == NULL)
	{
		return jobs_fg (jobs, 0);
	}
		
	if (*(argv + 2) != NULL)
	{
		fprintf (stderr, "fg needs a PID argument\n");
		return -1;
	}

	return jobs_fg (jobs, atoi (*(argv + 1)));
}

/*
 * Lists running jobs
 */
int
builtin_jobs (char * const *argv,
	      Jobs *jobs)
{
	if (*(argv + 1) != NULL)
	{
		fprintf (stderr, "jobs takes no arguments\n");
		return -1;
	}

	jobs_print (jobs);
	return 0;
}

/*
 * Prints help
 */
int
builtin_help (char * const *argv)
{
	printf ("SHELL HELP\n"
		"\n"
		"Type commands at the prompt and press ENTER to execute them\n"
		"\n"
		"Appending '&' to a command will run it in the background.\n"
		"Pressing Ctrl-Z while running a command will place it in the "
		 "background.\n"
		"\n"
		"Built-in commands:\n"
		"cd [dir]      change current working directory to dir\n"
		"exit          quit shell\n"
		"fg job        run job in foreground\n"
		"jobs          list background jobs\n"
		"history       list recently-typed commands\n"
		"r [cmd]       run recently-typed command beginning with cmd\n"
	       );
	
	return 0;
}

/*
 * Lists the most recently typed commands
 */
int
builtin_history (char * const *argv,
		 History *history)
{
	if (*(argv + 1) != NULL)
	{
		fprintf (stderr, "history takes no arguments\n");
		return -1;
	}

	history_print (history);
	return 0;
}
