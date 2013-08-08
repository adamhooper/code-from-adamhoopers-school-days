#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"
#include "job.h"
#include "client-thread.h"
#include "printer-thread.h"

static int
real_main (unsigned int num_clients,
	   unsigned int num_printers,
	   unsigned int buffer_size)
{
	Queue *queue;
	unsigned int i;
	pthread_t ignored;
	ClientThreadData *client_data;
	PrinterThreadData *printer_data;

	/* Initialize global data */
	queue = queue_new_with_size (buffer_size);
	if (queue == NULL)
	{
		fprintf (stderr, "Could not allocate queue\n");
		return -1;
	}

	srand (time (NULL));

	/* Create threads */
	for (i = 0; i < num_clients; i++)
	{
		client_data = malloc (sizeof (ClientThreadData));
		if (client_data == NULL)
		{
			fprintf (stderr, "Could not allocate thread data\n");
			exit (1);
		}

		client_data->client_id = i;
		client_data->queue = queue;
		pthread_create (&ignored, NULL, client_thread, client_data);
	}

	for (i = 0; i < num_printers; i++)
	{
		printer_data = malloc (sizeof (PrinterThreadData));
		if (printer_data == NULL)
		{
			fprintf (stderr, "Could not allocate thread data\n");
			exit (1);
		}

		printer_data->printer_id = i;
		printer_data->queue = queue;
		pthread_create (&ignored, NULL, printer_thread, printer_data);
	}

	/* Wait until all threads have finished */
	/*
	 * We don't actually have to do this! The program runs forever.
	 *
	 * To run forever, just try to join a thread which we know will never
	 * close. None of the above-created threads will.
	 */
	pthread_join (ignored, NULL);

	/* Finalize global data */
	queue_free (queue);

	return 0;
}

void
print_usage (const char *name)
{
	fprintf (stderr, "usage: %s num_clients num_printers buffer_size\n",
		 name);
}

int
main (int argc,
      char **argv)
{
	unsigned int num_clients;
	unsigned int num_printers;
	unsigned int buffer_size;

	if (argc != 4)
	{
		print_usage (argv[0]);
		return -1;
	}

	num_clients = (unsigned int) atoi (argv[1]);
	if (num_clients == 0)
	{
		fprintf (stderr, "Invalid num_clients '%s'\n", argv[1]);
		print_usage (argv[0]);
		return -1;
	}

	num_printers = (unsigned int) atoi (argv[2]);
	if (num_printers == 0)
	{
		fprintf (stderr, "Invalid num_printers '%s'\n", argv[2]);
		print_usage (argv[0]);
		return -1;
	}

	buffer_size = (unsigned int) atoi (argv[3]);
	if (buffer_size == 0)
	{
		fprintf (stderr, "Invalid buffer size '%s'\n", argv[3]);
		print_usage (argv[0]);
		return -1;
	}

	return real_main (num_clients, num_printers, buffer_size);
}
