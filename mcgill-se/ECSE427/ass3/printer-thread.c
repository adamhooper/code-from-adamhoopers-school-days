#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "printer-thread.h"
#include "job.h"

void *
printer_thread (void *data)
{
	PrinterThreadData *real_data;
	Queue *queue;
	Job *job;
	QueueEntry *entry;
	unsigned int printer_id;

	real_data = (PrinterThreadData *) data;

	queue = real_data->queue;
	printer_id = real_data->printer_id;

	free (real_data);

	while (1)
	{
		entry = queue_poll (queue);
		job = (Job *) queue_entry_get_data (entry);

		printf ("\tPrinter %d starts printing %d pages from "
			"Buffer [%d]\n",
			printer_id, job_get_num_pages (job),
			queue_entry_get_buffer_slot (entry));

		sleep (job_get_num_pages (job));

		job_free (job);
		queue_entry_free (entry);
	}

	return NULL;
}
