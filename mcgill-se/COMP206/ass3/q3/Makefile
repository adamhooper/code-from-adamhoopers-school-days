CFLAGS = -Wall -g

SOURCES = main.c bank.c

run: $(SOURCES:.c=.o)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $<

clean:
	rm -f run *.o
