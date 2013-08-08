#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "jobs.h"
#include "list.h"

struct _Jobs
{
	List *jobs;
};

/* Job: private object, to fit into Jobs's list */

typedef struct
{
	Cmd *cmd;
	struct termios ios;
	pid_t pid;
} Job;

static Job *
job_new (Cmd *cmd)
{
	Job *job = calloc (1, sizeof (Job));

	job->cmd = cmd_copy (cmd);
	tcgetattr (STDIN_FILENO, &job->ios);

	return job;
}

static void
job_free (Job *job)
{
	if (job != NULL)
	{
		cmd_free (job->cmd);
		free (job);
	}
}

static void
job_print (Job *job)
{
	printf ("[%ld] ", (long) job->pid);
	cmd_print (job->cmd);
}

static void
job_exec (Job *job)
{
	char * const *argv;

	signal (SIGINT, SIG_DFL);
	signal (SIGQUIT, SIG_DFL);
	signal (SIGTSTP, SIG_DFL);
	signal (SIGTTIN, SIG_DFL);
	signal (SIGTTOU, SIG_DFL);
	signal (SIGCHLD, SIG_DFL);

	argv = cmd_get_argv (job->cmd);

	execvp (argv[0], argv);

	fprintf (stderr, "Command failed\n");

	exit (1);
}

/*
 * Return values:
 *	-1: Error
 *	 0: Completed
 *	 1: Still running
 */
static int
run_in_foreground (Job *job)
{
	struct termios ios;
	int status;
	int still_running;

	tcgetattr (STDIN_FILENO, &ios);
	tcsetattr (STDIN_FILENO, TCSADRAIN, &job->ios);
	tcsetpgrp (STDIN_FILENO, job->pid);

	if (waitpid (job->pid, &status, WUNTRACED) == -1)
	{
		perror ("[run_in_foreground] waitpid");
	}

	tcgetattr (STDIN_FILENO, &job->ios);
	tcsetpgrp (STDIN_FILENO, getpid ());
	tcsetattr (STDIN_FILENO, TCSADRAIN, &ios);

	still_running = WIFSTOPPED (status) && WSTOPSIG (status) == SIGTSTP;

	if (still_running)
	{
		/* Ctrl-Z: run in background */
		kill (job->pid, SIGCONT);
		return 1;
	}

	return 0;
}

/*
 * Return values:
 *	 -1: error
 *	  0: job completed (entirely in foreground)
 *	PID: job not completed
 */
static pid_t
job_launch (Job *job)
{
	pid_t child;
	int bg;
	int unfinished;

	bg = cmd_get_background (job->cmd);

	child = fork ();

	if (child < 0)
	{
		perror ("fork");
		return child;
	}

	/* Run in both child and parent (avoid race) */
	setpgid (child, child);

	/* Now break apart: exec in child... */
	if (child == 0)
	{
		if (bg == 0)
		{
			/* Redundant: in case this fork comes first */
			tcsetpgrp (STDIN_FILENO, getpid ());
		}

		job_exec (job);
	}

	/* ... and clean up in parent */
	job->pid = child;

	unfinished = 0;

	if (bg == 0)
	{
		unfinished = run_in_foreground (job);
	}
	else
	{
		unfinished = 1;
	}

	return unfinished ? child : 0;
}

/* Jobs: manage Job objects */

/*
 * Returns a new Jobs object.
 */
Jobs *
jobs_new (void)
{
	return calloc (1, sizeof (Jobs));
}

/*
 * Begins a new job
 *
 * If the job is to be run in the background, control will return immediately.
 * Otherwise, control will return when the job is stopped or terminated.
 */
void
jobs_add (Jobs *jobs,
	  Cmd *cmd)
{
	Job *job = job_new (cmd);
	pid_t pid;

	pid = job_launch (job);

	if (pid < 0) return;

	if (pid == 0) /* already finished */
	{
		job_free (job);
		return;
	}

	jobs->jobs = list_append (jobs->jobs, job);

	printf ("Job started: ");
	job_print (job);
}

/*
 * Return values:
 * 	-1: error
 * 	 0: job did not complete
 * 	 1: job completed
 */
static int
wait_for_job (Job *job,
	      int block)
{
	pid_t pid;
	int status;

	pid = waitpid (job->pid, &status, block ? 0 : WNOHANG);

	if (pid == 0)
	{
		return 0;
	}

	if (pid == -1)
	{
		perror ("[wait_for_job] waitpid");
		return -1;
	}

#if DEBUG
	if (!WIFEXITED (status) && !WIFSIGNALED (status))
	{
		printf ("Unknown status: %x\n", status);
		return -1;
	}
#endif /* DEBUG */

	return 1;
}

static void
jobs_clean_real (Jobs *jobs,
		 int blocking)
{
	List *l;
	Job *j;

	for (l = jobs->jobs; l;)
	{
		j = (Job *) l->data;

		l = l->next; /* list_remove at bottom of loop */

		if (wait_for_job (j, blocking) != 0)
		{
			printf ("Job finished: ");
			job_print (j);

			jobs->jobs = list_remove (jobs->jobs, j);
			job_free (j);
		}
	}
}

/*
 * Cleans up zombie processes
 */
void
jobs_clean (Jobs *jobs)
{
	jobs_clean_real (jobs, 0);
}

/*
 * Bring job specified by pid to the foreground.
 *
 * If pid == 0, bring the most recently started job to the foreground.
 *
 * Returns 0 on success -1 on failure
 */
int
jobs_fg (Jobs *jobs,
	 pid_t pid)
{
	Job *j;
	List *l;

	if (jobs->jobs == NULL)
	{
		fprintf (stderr, "No jobs are running\n");
		return -1;
	}

	for (l = jobs->jobs; l; l = l->next)
	{
		j = (Job *) l->data;

		if (j->pid == pid
		    || (pid == 0 && l->next == NULL))
		{
			printf ("Foreground: ");
			job_print (j);

			if (run_in_foreground (j) == 0)
			{
				printf ("Job finished: ");
				job_print (j);

				jobs->jobs = list_remove (jobs->jobs, j);
				job_free (j);
			}
			else
			{
				printf ("Background: ");
				job_print (j);
			}

			return 0;
		}
	}

	fprintf (stderr, "Job %ld not found\n", (long) pid);
	return -1;
}

static void
multiplex_signal (Jobs *jobs,
		  int sig)
{
	Job *j;
	List *l;

	for (l = jobs->jobs; l; l = l->next)
	{
		j = (Job *) l->data;

		kill (j->pid, sig);
	}
}

/*
 * Frees all memory and terminates all processes associated with jobs
 */
void
jobs_free (Jobs *jobs)
{
#define TRY_JOBS_EMPTY(block)			\
	do {					\
		jobs_clean_real (jobs, block);	\
		if (jobs->jobs == NULL)		\
		{				\
			free (jobs);		\
			return;			\
		}				\
	} while (0)			

	TRY_JOBS_EMPTY(0);

#if DEBUG
	printf ("Sending jobs TERM signal...\n");
#endif /* DEBUG */
	multiplex_signal (jobs, SIGTERM);

	sleep (1);

	TRY_JOBS_EMPTY(0);

#if DEBUG
	printf ("Sending jobs KILL signal...\n");
#endif /* DEBUG */
	multiplex_signal (jobs, SIGKILL);

	TRY_JOBS_EMPTY(1);

#undef TRY_JOBS_EMPTY
}

/*
 * Prints a textual description of jobs
 */
void
jobs_print (Jobs *jobs)
{
	List *l;

	printf ("Active jobs:\n");

	for (l = jobs->jobs; l; l = l->next)
	{
		job_print ((Job *) l->data);
	}
}
