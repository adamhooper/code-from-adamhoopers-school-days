all: disk.o sfs_api.o dt

sfs_api.o: sfs_api.c sfs_api.h
	gcc -Wall -c sfs_api.c

disk.o: disk.c disk.h
	gcc -Wall -c disk.c

dt: disk_test.c disk.o sfs_api.o
	gcc -Wall disk_test.c disk.o sfs_api.o -o dt
