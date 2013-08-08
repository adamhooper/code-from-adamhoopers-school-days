CC = gcc
CFLAGS = -g -Wall -ansi -pedantic -D_POSIX_SOURCE #-DDEBUG

SOURCE_FILES = \
	builtin.c	\
	cmd.c		\
	main.c		\
	history.c	\
	jobs.c		\
	list.c

shell: $(SOURCE_FILES:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@

builtin.c: builtin.h
cmd.c: cmd.h list.h
history.c: history.h list.h
jobs.c: jobs.h list.h
list.c: list.h
main.c: builtin.h cmd.h history.h jobs.h

builtin.h: history.h jobs.h
cmd.h:
history.h: cmd.h
jobs.h: cmd.h
list.h:

%.o: %.c
	gcc $(CFLAGS) -c $<

clean:
	rm -f *.o shell
