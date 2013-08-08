#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include "queue.h"

typedef struct _ClientThreadData ClientThreadData;

struct _ClientThreadData
{
	unsigned int client_id;
	Queue *queue;
};

/*
 * Pointer to pass to pthread_create(). Must be passed a ClientThreadData *.
 *
 * The passed ClientThreadData * will be freed by the thread.
 */
void	*client_thread (void *data);

#endif /* CLIENT_THREAD_H */
