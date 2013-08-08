#include <stdlib.h>

#include "job.h"

struct _Job
{
	unsigned int num_pages;
};

Job *
job_new (unsigned int num_pages)
{
	Job *ret;

	ret = malloc (sizeof (Job));
	if (ret == NULL)
	{
		return NULL;
	}

	ret->num_pages = num_pages;

	return ret;
}

unsigned int
job_get_num_pages (Job *job)
{
	return job->num_pages;
}

void
job_free (Job *job)
{
	free (job);
}
