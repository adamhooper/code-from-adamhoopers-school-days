#ifndef _BUILTINS_H
#define _BUILTINS_H

/*
 * Built-in shell commands.
 *
 * Common return values:
 *	<0: erroneous command (i.e., bad parameters)
 *	=0: successfully ran command
 *	>0: error (possibly errno)
 */

#include "history.h"
#include "jobs.h"

int	builtin_cd	(char * const *argv);
int	builtin_fg	(char * const *argv, Jobs *jobs);
int	builtin_jobs	(char * const *argv, Jobs *jobs);
int	builtin_help	(char * const *argv);
int	builtin_history	(char * const *argv, History *history);

#endif /* _BUILTINS_H */
