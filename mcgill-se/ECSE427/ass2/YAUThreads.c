#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>
                                                                              
#include "YAUThreads.h"

#define MAX_THREADS			32
#define THREAD_STACK_SIZE		1024*64
#define QUANTUM				1 /* in seconds */

struct _threaddesc
{
	unsigned int id;
	unsigned int priority;
	ucontext_t context;
};

static SchedulingPolicy sched_policy;
static threaddesc threadarr[MAX_THREADS];
static unsigned int numthreads, curthread;
static ucontext_t parent;
static unsigned int closing_thread; /* 1 to clean up after last thread */
static void *last_stack_to_free;

void initYAUThreads()
{
	numthreads = 0;
	curthread = 0;
	closing_thread = 0;
	last_stack_to_free = NULL;
}

int YAUSpawn(void *threadfunc, unsigned int priority)
{
	threaddesc *tdescptr;

	if (numthreads >= MAX_THREADS) 
	{
		printf("Maximum thread limit reached... creation failed! \n");
		return -1;
	}

	if (priority < 1) {
		priority = 1;
	}

	tdescptr = &(threadarr[numthreads]);
	getcontext(&(tdescptr->context));
	tdescptr->id = numthreads;
	tdescptr->priority = priority;
	tdescptr->context.uc_stack.ss_sp = malloc (THREAD_STACK_SIZE);
	tdescptr->context.uc_stack.ss_size = THREAD_STACK_SIZE;
	tdescptr->context.uc_link = NULL;
	tdescptr->context.uc_stack.ss_flags = 0;

	makecontext(&(tdescptr->context), threadfunc, 0);

	numthreads++;

	return 0;
}

/*
 * Removes a thread from threadarr
 */
static void removeThread(unsigned int thread_num) {
	unsigned int i;
	threaddesc *th;

	th = &threadarr[thread_num];

	numthreads--;

	if (numthreads == 0)
	{
		/*
		 * If numthreads == 0, it means the signal handler is being
		 * called on top of the currently-running stack; we can't
		 * free it yet, but we could free it in YAUWaitall().
		 *
		 * This hack is more of a "this actually makes it work" thing
		 * than a properly-researched phenomenon. It's hopeless to try
		 * and come up with a reasonable explanation as to why it would
		 * crash if we don't do this.
		 */
		last_stack_to_free = th->context.uc_stack.ss_sp;
		return;
	}

	free (th->context.uc_stack.ss_sp);

	if (numthreads == 0) return;

	if (curthread >= thread_num)
	{
		curthread = (curthread - 1 + numthreads) % numthreads;
	}

	for (i = thread_num; i < numthreads; i++)
	{
		threadarr[i] = threadarr[i + 1];
	}
}

static void handle_timerexpiry() 
{
	/*
	 * Switch to the next thread. If closing_thread is non-zero, close the
	 * last-run thread.
	 */
	unsigned int curthreadsave;
	unsigned int closing_tmp;

	closing_tmp = closing_thread;

	if (closing_thread) {
		/*
		 * free() isn't safe to call within a signal handler. But
		 * neither is swapcontext() -- this entire assignment is
		 * founded on a disgusting hack, so what the hell.
		 */
		removeThread(curthread);
		closing_thread = 0;
	}

	if (numthreads == 0) {
		setcontext(&parent);
	}

	curthreadsave = curthread;
	curthread = (curthread + 1) % numthreads;

	signal(SIGALRM, &handle_timerexpiry);

	if (sched_policy != FCFS) {
		alarm (QUANTUM * threadarr[curthread].priority);
	}

	if (closing_tmp) {
		setcontext(&(threadarr[curthread].context));
	}
	else
	{
		swapcontext(&(threadarr[curthreadsave].context),
			    &(threadarr[curthread].context));
	}
}

static int thread_cmp (const void *a, const void *b)
{
	threaddesc *ta = (threaddesc *) a;
	threaddesc *tb = (threaddesc *) b;

	/* Highest to lowest */
	return tb->priority - ta->priority;
}

static void reorder_threads (void)
{
	qsort (threadarr, numthreads, sizeof (threaddesc), thread_cmp);
}

void startYAUThreads(SchedulingPolicy policy)
{
	sched_policy = policy;

	if (numthreads <= 0)
	{
		printf ("No threads to run\n");
		return;
	}

	if (sched_policy == PRIORITY)
	{
		reorder_threads();
	}

	signal(SIGALRM, &handle_timerexpiry);

	if (sched_policy != FCFS)
	{
		alarm (QUANTUM * threadarr[curthread].priority);
	}

	swapcontext(&parent, &(threadarr[curthread].context));
}

unsigned int getYAUThreadid()
{
	return threadarr[curthread].id;
}

void closeYAUThread()
{
	alarm(0);

	closing_thread = 1;

	raise(SIGALRM);
}
		
void YAUWaitall()
{
	/*
	 * This only gets called after all threads are closed, anyway. There's
	 * not much left to clean up.
	 */
	if (last_stack_to_free != NULL)
	{
		free (last_stack_to_free);
		last_stack_to_free = NULL;
	}
}
