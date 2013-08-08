#ifndef DISK_H
#define DISK_H
#include <stdio.h>
// disk format
#define NUM_BLOCKS 64
#define BLOCK_SIZE 2048

// disk error probability
#define RELIABILITY 0.95
// just some constants
#define NULL_CHAR '\0'
#define YES 1
#define NO 0
// error conditions
#define SUCCESS 0
#define DISK_ACCESS_ERROR 1
#define DISK_SEEK_ERROR 2


int initDisk();
int finishDisk();
int dRead(int addr, char *buf);
int dWrite(int addr, char *buf);

#endif
