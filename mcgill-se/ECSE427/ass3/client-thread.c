#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

#include "client-thread.h"
#include "job.h"

#define REQUEST_INTERVAL 5

/*
 * Do not use rand(): it is not thread-safe
 *
 * This one returns a number from 1 to 10
 */
static unsigned int
print_rand (void)
{
	unsigned int ret;
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock (&mutex);

	ret = (int) ((double) rand() / ((double) RAND_MAX + 1) * 10);

	pthread_mutex_unlock (&mutex);

	return ret;
}

void *
client_thread (void *data)
{
	ClientThreadData *real_data;
	Job *job;
	Queue *queue;
	QueueEntry *entry;
	unsigned int client_id;

	real_data = (ClientThreadData *) data;

	queue = real_data->queue;
	client_id = real_data->client_id;

	free (real_data);

	while (1)
	{
		job = job_new (print_rand() % 10 + 1);
		if (job == NULL)
		{
			fprintf (stderr, "Failed to allocate job\n");
			return NULL;
		}

		entry = queue_entry_new (job);
		if (entry == NULL)
		{
			fprintf (stderr, "Failed to allocate queue entry\n");
			job_free (job);
			return NULL;
		}

		queue_offer (queue, entry);

		printf ("Client %d has %d pages to print, puts request in "
			"Buffer [%d]\n",
			client_id, job_get_num_pages (job),
			queue_entry_get_buffer_slot (entry));

		sleep (REQUEST_INTERVAL);
	}

	return NULL;
}
