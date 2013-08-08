CC=gcc
CFLAGS=-O3 -g -Wall `pkg-config --cflags glib-2.0`
LIBS=`pkg-config --libs glib-2.0`

solve: solve.c
	$(CC) $(CFLAGS) $(LIBS) solve.c -o solve
