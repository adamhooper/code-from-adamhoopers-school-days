CFLAGS=`pkg-config --cflags glib-2.0` -Wall -pg -g -O3
LIBS=`pkg-config --libs glib-2.0`

all: solve

clean:
	rm *.o torture solve

%.c: solve.h

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

solve: solve_backend.o solve.o
	gcc -pg $(LIBS) solve_backend.o solve.o -o $@

torture: solve_time.o solve_backend.o
	gcc -pg $(LIBS) solve_time.o solve_backend.o -o $@
