#ifndef _CMD_H
#define _CMD_H

/*
 * Represents an individual shell command.
 *
 * Commands are created using cmd_parse() on a line of input.
 * cmd_get_argv() will return the parsed argument list.
 * cmd_get_background() will return whether to start in the background.
 */

#include <sys/types.h>

typedef struct _Cmd Cmd;

Cmd		*cmd_parse		(const char *s);
Cmd		*cmd_copy		(Cmd *cmd);
char * const	*cmd_get_argv		(Cmd *cmd);
int		 cmd_get_background	(Cmd *cmd);
pid_t		 cmd_run		(Cmd *cmd);
void		 cmd_free		(Cmd *cmd);
void		 cmd_print		(Cmd *cmd);

#endif /* _CMD_H */
