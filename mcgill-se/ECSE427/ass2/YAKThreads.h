#ifndef __YAKTHREAD_H__
#define __YAKTHREAD_H__

/*
 * Initialize the threading library.
 */
void initYAKThreads();

/*
 * Spawn a new thread for function func.
 */
int YAKSpawn(void *func);

/*
 * Returns a unique identifier for the currently-running thread.
 */
int getYAKThreadid(void);

/*
 * Call from within a thread to close the thread.
 *
 * All thread functions must end with a call to closeYAKThread().
 */
void closeYAKThread();

/*
 * Wait for all spawned threads to complete.
 */
void YAKWaitall();

#endif
