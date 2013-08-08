CC = gcc
CFLAGS = -Wall -g
YAULIBS = -L. -lYAU
YAKLIBS = -L. -lYAK

all: threadlib

threadlib: libYAU.a libYAK.a

test: testYAU testYAUPolicy testYAK

libYAU.a: YAUThreads.o
	ar rcs libYAU.a YAUThreads.o

libYAK.a: YAKThreads.o list.o
	ar rcs libYAK.a $^

%.o: %.c
	gcc $(CFLAGS) -c $<

testYAU: testYAU.c libYAU.a
	gcc $(CFLAGS) -o $@ $< $(YAULIBS)

testYAUPolicy: testYAUPolicy.c libYAU.a
	gcc $(CFLAGS) -o $@ $< $(YAULIBS)

testYAK: testYAK.c libYAK.a
	gcc $(CFLAGS) -o $@ $< $(YAKLIBS)

clean:
	rm -f *.a *.o testYAU testYAUPolicy testYAK
