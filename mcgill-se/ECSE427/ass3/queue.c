#include <pthread.h>
#include <stdlib.h>

#include "queue.h"

#define DEFAULT_QUEUE_SIZE 32

/*
 * For a queue of n elements, only (n - 1) may be full at any time (otherwise
 * we wouldn't be able to tell from first and last whether the queue is full or
 * empty).
 *
 * In this implementation, when first = last the queue is empty; last actually
 * marks one index *after* the last entry.
 */

struct _QueueEntry
{
	void *data;
	unsigned int buffer_slot;
};

QueueEntry *
queue_entry_new (void *data)
{
	QueueEntry *entry;

	entry = malloc (sizeof (QueueEntry));
	if (entry == NULL)
	{
		return NULL;
	}

	entry->data = data;

	return entry;
}

void *
queue_entry_get_data (QueueEntry *entry)
{
	return entry->data;
}

unsigned int
queue_entry_get_buffer_slot (QueueEntry *entry)
{
	return entry->buffer_slot;
}

void
queue_entry_free (QueueEntry *entry)
{
	free (entry);
}

struct _Queue
{
	QueueEntry **data;
	size_t max_size;
	unsigned int first;
	unsigned int size;
	pthread_cond_t not_empty_cond;
	pthread_cond_t not_full_cond;
	pthread_mutex_t mutex;
};

Queue *
queue_new_with_size (size_t max_size)
{
	Queue *queue;

	queue = malloc (sizeof (Queue));
	if (queue == NULL)
	{
		return NULL;
	}

	queue->data = malloc (max_size * sizeof (QueueEntry *));
	if (queue->data == NULL)
	{
		free (queue);
		return NULL;
	}

	queue->max_size = max_size;
	queue->first = 0;
	queue->size = 0;
	pthread_mutex_init (&queue->mutex, NULL);
	pthread_cond_init (&queue->not_empty_cond, NULL);
	pthread_cond_init (&queue->not_full_cond, NULL);

	return queue;
}

Queue *
queue_new (void)
{
	return queue_new_with_size (DEFAULT_QUEUE_SIZE);
}

void
queue_free (Queue *queue)
{
	pthread_cond_destroy (&queue->not_empty_cond);
	pthread_cond_destroy (&queue->not_full_cond);
	pthread_mutex_destroy (&queue->mutex);
	free (queue->data);
	free (queue);
}

void
queue_offer (Queue *queue,
	     QueueEntry *entry)
{
	unsigned int slot;

	pthread_mutex_lock (&queue->mutex);

	while (queue->size == queue->max_size)
	{
		pthread_cond_wait (&queue->not_full_cond, &queue->mutex);
	}

	slot = (queue->first + queue->size) % queue->max_size;
	queue->size++;

	entry->buffer_slot = slot;
	queue->data[slot] = entry;

	pthread_cond_signal (&queue->not_empty_cond);

	pthread_mutex_unlock (&queue->mutex);
}

QueueEntry *
queue_poll (Queue *queue)
{
	QueueEntry *entry;

	pthread_mutex_lock (&queue->mutex);

	while (queue->size == 0)
	{
		pthread_cond_wait (&queue->not_empty_cond, &queue->mutex);
	}

	entry = queue->data[queue->first];

	queue->first = (queue->first + 1) % queue->max_size;
	queue->size--;

	pthread_cond_signal (&queue->not_full_cond);

	pthread_mutex_unlock (&queue->mutex);

	return entry;
}
