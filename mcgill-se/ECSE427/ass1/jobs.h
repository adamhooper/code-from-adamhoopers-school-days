#ifndef _JOBS_H
#define _JOBS_H

/*
 * Manages all jobs.
 *
 * A job is started by running jobs_add() with a Cmd object. If a job is
 * started in or moved to the background, control is returned to the caller and
 * the job's PID is returned (if the job never goes to the background, the
 * return value is 0).
 *
 * To put a job back in the foreground, run jobs_fg() with the PID returned
 * from jobs_add().
 *
 * Run jobs_clean() to clean up zombie processes.
 */

#include <sys/types.h>

#include "cmd.h"

typedef struct _Jobs Jobs;

Jobs	*jobs_new	(void);
void	 jobs_add	(Jobs *jobs, Cmd *cmd);
int	 jobs_fg	(Jobs *jobs, pid_t pid);
void	 jobs_clean	(Jobs *jobs);
void	 jobs_free	(Jobs *jobs);
void	 jobs_print	(Jobs *jobs);

#endif /* _JOBS_H */
