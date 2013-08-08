#ifndef __YAUTHREAD_H__
#define __YAUTHREAD_H__

typedef enum {
	RR,		/* round robin */
	FCFS,		/* first come first served */
	PRIORITY,	/* priority-based (preemptive) */
} SchedulingPolicy;

typedef struct _threaddesc threaddesc;

/*
 * Initialize the threading library.
 */
void initYAUThreads();

/*
 * Spawn a new thread for function threadfunc, with priority priority.
 *
 * Higher priority threads are scheduled earlier and get more CPU time.
 */
int YAUSpawn(void *threadfunc, unsigned int priority);

/*
 * Run spawned threads until all of them are complete.
 */
void startYAUThreads(SchedulingPolicy policy);

/*
 * Call from within a thread to determine the currently-running thread's ID.
 */
unsigned int getYAUThreadid();

/*
 * Call from within a thread to close the thread.
 *
 * All thread functions must end with a call to closeYAUThread().
 */
void closeYAUThread();

/*
 * Clean up after startYAUThreads().
 */
void YAUWaitall();

#endif
