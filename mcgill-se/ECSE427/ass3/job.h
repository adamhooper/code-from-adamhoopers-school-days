#ifndef JOB_H
#define JOB_H

typedef struct _Job Job;

Job		*job_new		(unsigned int num_pages);

unsigned int	 job_get_num_pages	(Job *job);

void		 job_free		(Job *job);

#endif /* JOB_H */
