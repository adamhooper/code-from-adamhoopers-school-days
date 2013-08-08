CC = gcc
CFLAGS = -g -Wall -ansi -pedantic -D_POSIX_SOURCE
LIBS = -lpthread

all: print

print.c: queue.h client-thread.h printer-thread.h
client-thread.c: client-thread.h job.h
printer-thread.c: printer-thread.h job.h
queue.c: queue.h
job.c: job.h
client-thread.h: queue.h
printer-thread.h: queue.h

print: print.o queue.o job.o client-thread.o printer-thread.o
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

clean:
	rm -f *.o print

%.o: %.c
	$(CC) $(CFLAGS) -c $<
