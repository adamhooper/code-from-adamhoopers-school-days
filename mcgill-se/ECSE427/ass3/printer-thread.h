#ifndef PRINTER_THREAD_H
#define PRINTER_THREAD_H

#include "queue.h"

typedef struct _PrinterThreadData PrinterThreadData;

struct _PrinterThreadData
{
	unsigned int printer_id;
	Queue *queue;
};

/*
 * Pointer to pass to pthread_create(). Must be passed a PrinterThreadData *.
 *
 * The passed PrinterThreadData * will be freed by the thread.
 */
void	*printer_thread (void *data);

#endif /* PRINTER_THREAD_H */
