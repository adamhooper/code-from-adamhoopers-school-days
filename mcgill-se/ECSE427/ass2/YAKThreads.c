#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "list.h"
#include "YAKThreads.h"

#define THREAD_STACK_SIZE	1024*64

typedef struct
{
	pid_t pid;
	void *stack;
} Thread;

/*
 * Maintain a linked list of all running threads. When a thread is closed, we
 * remove it from the list in YAKWaitall(), which is run from the main thread.
 */
static List *threads;

static Thread *
thread_new (pid_t pid,
	    void *stack)
{
	Thread *ret;

	ret = malloc (sizeof (Thread));
	if (ret == NULL)
	{
		perror ("thread_new: malloc");
		return NULL;
	}

	ret->pid = pid;
	ret->stack = stack;

	return ret;
}

static void
thread_free (Thread *thread)
{
	free (thread->stack);
	free (thread);
}

void
initYAKThreads (void)
{
	threads = NULL;
}

int
YAKSpawn (void *func)
{
	Thread *thread;
	void *stack;
	pid_t pid;

	stack = malloc (THREAD_STACK_SIZE);
	if (stack == NULL)
	{
		perror ("YAKSpawn: malloc");
		return -1;
	}

	pid = clone (func, stack + THREAD_STACK_SIZE,
		     SIGCHLD | CLONE_FS | CLONE_FILES
		     | CLONE_SIGHAND | CLONE_VM, 0);
	if (pid < 0)
	{
		perror ("YAKSpawn: clone");
		free (stack);
		return pid;
	}

	thread = thread_new (pid, stack);

	threads = list_prepend (threads, thread);

	return 0;
}

int
getYAKThreadid (void)
{
	return getpid();
}

void
closeYAKThread (void)
{
	/*
	 * Don't use exit(0) -- we don't want to flush stdio
	 */
	_exit (0);
}

static void
remove_thread (pid_t pid)
{
	Thread *thread;
	List *l;

	for (l = threads; l != NULL; l = l->next)
	{
		thread = (Thread *) l->data;

		if (thread->pid == pid)
		{
			threads = list_remove (threads, thread);
			thread_free (thread);
			return;
		}
	}
}

void
YAKWaitall (void)
{
	pid_t pid;

	while (threads != NULL)
	{
		pid = wait (NULL);

		if (pid == -1)
		{
			fprintf (stderr, "Library broken while waiting for threads!\n");
			return;
		}

		remove_thread (pid);
	}
}
