#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "disk.h"

const char *diskFileName = "diskSimulate.dat";
static FILE *diskFile;

int initDisk(int fresh) {
  int i, j, init = YES;
  char nullChar = NULL_CHAR;

  srand((unsigned int)time(NULL));
  if (access(diskFileName, F_OK) == 0) {
    // check whether the the simulated disk file is already there
    if (!fresh) {
      init = NO;
      if ((diskFile = fopen(diskFileName, "r+")) == NULL) {
	printf("Error in opening the existing disk\n");
	return DISK_ACCESS_ERROR;
      }
    }
  }
  if (init == YES) {
    if (!fresh) {
      printf("Trying to open non-existing disk!\n");
      return DISK_ACCESS_ERROR;
    } else {
      // create/format the disk
      if ((diskFile = fopen(diskFileName, "w+")) == NULL) {
	printf("Error in initializing the disk\n");
	return DISK_ACCESS_ERROR;
      } else {
	// create the disk space empty data
	for (i=0; i<NUM_BLOCKS; i++)
	  for (j=0; j<BLOCK_SIZE; j++)
	    fwrite(&nullChar, (size_t)sizeof(char), 1, diskFile);
	//sleep(3);
      }
    }
  }
  return SUCCESS;
}

int finishDisk() {
  //just to close the file pointer
  if (diskFile != NULL) {
    fclose(diskFile);
    return SUCCESS;
  } else {
    return DISK_SEEK_ERROR;
  }
}

int dWrite(int addr, char *buf) {
  if (addr >= NUM_BLOCKS) {
    printf("Trying to access outside the disk space\n");
    return DISK_SEEK_ERROR;
  }
  if (rand() > RELIABILITY*RAND_MAX)
    // random error messages
    return DISK_ACCESS_ERROR;

  // position the file pointer at the required position
  if (fseek(diskFile, (long)(addr*BLOCK_SIZE), SEEK_SET) != 0) {
    printf("Disk access error!\n");
    return DISK_SEEK_ERROR; 
  }
  fwrite(buf, (size_t)sizeof(char), (size_t)BLOCK_SIZE, diskFile);
  return SUCCESS;
}
  
int dRead(int addr, char *buf) {
  if (addr >= NUM_BLOCKS) {
    printf("Trying to access outside the disk space\n");
    return DISK_SEEK_ERROR;
  }
  if (rand() > RELIABILITY*RAND_MAX)
    // random error messages
    return DISK_ACCESS_ERROR;

  // position the file pointer at the required position
  if (fseek(diskFile, (long)(addr*BLOCK_SIZE), SEEK_SET) != 0) {
    printf("Disk access error!\n");
    return DISK_SEEK_ERROR; 
  }
  fread(buf, (size_t)sizeof(char), (size_t)BLOCK_SIZE, diskFile);
  return SUCCESS;
}

