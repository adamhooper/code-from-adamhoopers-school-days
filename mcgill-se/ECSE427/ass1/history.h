#ifndef _HISTORY_H
#define _HISTORY_H

/*
 * Stores a history of recent commands.
 *
 * When running a new command, call history_append() with the Cmd.
 *
 * To find a history command, call history_find_cmd(). It will try to match the
 * given argument with each Cmd in history.
 *
 * Note that history does not go back forever; it has a maximum size. Old
 * history items are automatically removed.
 */

#include "cmd.h"

typedef struct _History History;

History	*history_new		(void);
void	 history_append		(History *h, Cmd *cmd);
Cmd	*history_find_cmd	(History *h, const char *argv0);
void	 history_free		(History *h);
void	 history_print		(History *h);

#endif /* _HISTORY_H */
