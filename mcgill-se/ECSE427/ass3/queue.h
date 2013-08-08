#ifndef QUEUE_H
#define QUEUE_H

/*
 * queue.h: thread-safe queues
 *
 * Queues may be offered new elements with queue_offer() and will return
 * elements with queue_poll(). Both operations are thread-safe and will block
 * if the queue is full or empty, respectively.
 */

typedef struct _Queue Queue;
typedef struct _QueueEntry QueueEntry;

/*
 * Returns a new QueueEntry, which can be placed in a Queue
 */
QueueEntry	*queue_entry_new		(void *data);

/*
 * Returns the data from this QueueEntry
 */
void		*queue_entry_get_data		(QueueEntry *entry);

/*
 * Returns the buffer slot of this QueueEntry.
 *
 * When a QueueEntry is inserted in a Queue using queue_offer (), this variable
 * is set. To call this function before queue_offer () is a bug.
 */
unsigned int	 queue_entry_get_buffer_slot	(QueueEntry *entry);

/*
 * Frees a QueueEntry
 */
void		 queue_entry_free		(QueueEntry *entry);

/*
 * Returns a new Queue of the given size
 */
Queue		*queue_new_with_size		(size_t size);

/*
 * Returns a new Queue with the default buffer size
 */
Queue		*queue_new			(void);

/*
 * Frees the memory allocated to a queue.
 *
 * This is a shallow free; be sure the queue is empty before freeing!
 */
void		 queue_free			(Queue *queue);

/*
 * Inserts a new element at the back of the queue.
 */
void		 queue_offer			(Queue *queue, QueueEntry *entry);

/*
 * Removes and returns the element at the front of the queue.
 *
 * The element must be freed using queue_entry_free().
 */
QueueEntry	*queue_poll			(Queue *queue);

#endif /* QUEUE_H */
