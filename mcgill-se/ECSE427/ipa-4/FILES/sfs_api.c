#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "disk.h"
#include "sfs_api.h"

/*
  SIMPLE FILE SYSTEM IMPLEMENTATION FILE
*/


//GLOBAL VARIABLES

//array of file descriptors
fdescTP fd_table[MAX_FDS]; 

//array of fileTP (file pointers)
fileTP root_dir[NUM_BLOCKS]; //will contain a malleable representation of each file in the directory
char dir_block[BLOCK_SIZE]; //will contain the direcotry data

//this variable will store the contents of the fat in main memory
char fat_block[BLOCK_SIZE];
fatTP FAT;



//since we cannot assume that the size of a struct is the sum of
//the sizes of all its attributes (due to padding), this function
//takes a fileT structure and serializes it into a string of bytes
//to save memory.
char* serialize_fileT(char* dest, fileTP source) {
  //the order in which the data will be stored is
  //filename-root_fat-size.
  char* curpos = dest;
  //copy name
  memcpy(curpos, &(source->name[0]),FILENAME_SIZE);
  curpos = curpos + FILENAME_SIZE;
  memcpy(curpos, &(source->root_fat), sizeof(ushort));
  curpos = curpos + sizeof(ushort);
  memcpy(curpos, &(source->size), sizeof(int));
  return dest;
}

//we do the opposite of serialize_fileT to construct a
//fileT structure from a string  
fileTP unserialize_fileT(fileTP dest, char* source){
  char* curpos = source;
  memcpy(&(dest->name[0]), curpos, FILENAME_SIZE);
  dest->name[FILENAME_SIZE] = '\0';
  curpos = curpos + FILENAME_SIZE;
  memcpy(&(dest->root_fat),curpos, sizeof(ushort));
  curpos = curpos + sizeof(ushort);
  memcpy(&(dest->size), curpos, sizeof(int));
  return dest;
}

/* This function calls dRead until success
   It catches a DISK_SEEK_ERROR if it happens and returns it
*/
int safe_dRead(int addr,char*buf) {
  int result=0;
  while((result=dRead(addr,buf))) { //loop stops on SUCCESS
    if (result == DISK_SEEK_ERROR) {
      //real disk problem
      return DISK_SEEK_ERROR;
    }
  }
  return result;
}

// This function calls dWrite until success
// it catches a DISK_SEEK_ERROR if it happens and returns it
int safe_dWrite(int addr, char *buf) {
  int result = 0;
  while((result=dWrite(addr,buf))) {
    if (result == DISK_SEEK_ERROR) {
      //real disk problem
      return DISK_SEEK_ERROR;
    }
  }
  return result;
}

int read_root_dir() {
  int filesread = 0; //number of files read from the dir
  char* cur_file; //points to a packed fileT in the block

  fileTP new_file;
  int cur_pos = 0; //position in buffer

  //check the first file in the block
  cur_file = &dir_block[0]; //points to address of current FCB in block
  while (cur_pos + FCB_SIZE <= BLOCK_SIZE) { //we read until end of block
    if (dir_block[cur_pos]) {
      
      //we found a valid file : If the first byte of a filename is null then the file is invalid
      filesread++;
      
      //we create a new node for the list      
      if ((new_file = malloc(sizeof(fileT))) == NULL) {
	printf("Insufficient memory to read directory.\n");
	return -2;
      }
      //unserialize_fileT takes a FCB_SIZE string (32 bytes) and converts it into
      //a more malleable fileT structure, which has name, size and root_fat attributes
      unserialize_fileT(new_file,cur_file);
      root_dir[new_file->root_fat] = new_file; //add the file with root_fat x at index x in the root_dir array

    }
    cur_file = cur_file + FCB_SIZE;
    cur_pos = cur_pos + FCB_SIZE;
  }
} //end read_root_dir
  


//this function initializes the in memory directory structure by cleaning/setting to
//NULL all the file entries in the in memory directory structure
int init_dir() {
  static char first_time = 1;

  if (first_time == 0) {
    //we erase the memory 
    int i;
    for(i=0;i<NUM_BLOCKS;i++) {
      if (root_dir[i] != NULL) {
	free(root_dir[i]); //free memory for the fileT struct
	root_dir[i] = NULL;
      }
    }
  }

  else {
    //this will only be run for the first time the function is called
    first_time = 0;
    int i;
    for (i=0;i<NUM_BLOCKS;i++) {
      root_dir[i] = NULL;
    }
  }    
  return 0; //success
} //end init_dir

//this function initializes the file descriptor table by making each of its members
//point to null. If its not the first time the function is called, then memory previously allocated is freed and then
//pointers are set to null.
void init_fd() {
  int i;
  static char first_time = 1;
  if (first_time == 1) {
    first_time = 0;
    for(i=0; i<MAX_FDS; i++) {
      fd_table[i] = NULL;
    }
  }
  else {
    for(i=0; i<MAX_FDS; i++) {
      if (fd_table[i] != NULL) {
	free(fd_table[i]);
	fd_table[i] = NULL;
      }
    }
  }
}
	
int read_fat_to_mem(){
  //copy the fat block into our global variable
  if (safe_dRead(0,&fat_block[0]) == DISK_SEEK_ERROR) return DISK_SEEK_ERROR;
  if ((FAT = malloc(sizeof(fatT)))== NULL) {
    printf("Not enough memory to initialize FAT.\n");
    exit(EXIT_FAILURE);
  }

  //make the array of short ints of the FAT point to the array of bytes from the
  //fat block that was copied into memory. The fat is reaaally more flexible that way
  //because we can access each index of the fat using the array
  FAT->next = (ushort*) &fat_block[0];

}

//This function writes in the last entry of the fat the next free block. And then
//for the next free block sets the pointer to the next free block... until it reaches the
//last free block. The next block of the last free block is block 0, which is used as a sentinel. 
//this routine needs to be run on a fresh virtual disk otherwise, it screws everything
int create_free_block_chain() {

  //the pointer to the list of free blocks is the last entry of the fat
  //note that this fat as one more entry than the number of blocks in the disk
  FAT->next[NUM_BLOCKS] = 2; //the first free_block is block 2
  
  //now we need to set the next pointers from blocks 2 to NUM_BLOCKS -1
  ushort cur_blk = 2;
  while(cur_blk < NUM_BLOCKS -1) {
    FAT->next[cur_blk] = cur_blk + 1;
    cur_blk++;
  }
  //the next pointer of the last free block is 0 (sentinel just to know the list is finished)
  FAT->next[cur_blk] = 0;
  if (safe_dWrite(0,&fat_block[0]) == DISK_SEEK_ERROR) { //write back the modified FAT to disk
    return DISK_SEEK_ERROR;
  }
  return 0; //success
}

//creates a new filesystem or reuses one
//returns -1 on error
//returns 0 on success	
int  mksfs(int fresh) {
  initDisk(fresh); //use a new disk, or reuse it depending on the value of fresh
  
  //read directory block
  if ( safe_dRead(1, &(dir_block[0])) == DISK_SEEK_ERROR) return -1;
  //store the fat inside memory
  read_fat_to_mem();
  
  init_fd(); //initialize the file descriptor table

  if (fresh) {	         
    create_free_block_chain();
    init_dir(); //initialize empty in memory directory structure
    //it is not necessary to read the disk directory since it is empty
  }
  else {
    read_root_dir();	//this will fill the in memory root_dir structure 
  }
}

void sfs_ls() {
  char* cur_file_addr = &(dir_block[0]); //this will point to the first byte of every file block
  int cur_pos = 0;
  fileTP new_file;
  int filesread = 0;
  int i = 0;
  for (i=0;i<NUM_BLOCKS;i++){
    if (root_dir[i]) { //if not null
      filesread++;
      printf("%s\t\t\t\t%d\n",&(root_dir[i]->name[0]),root_dir[i]->size);

    }
  }
  if ( filesread == 0) {
    printf("directory empty.\n");
  }
  else {
    printf("%d files.\n", filesread);
  }
}
  

fdescTP createFD(ushort rt_fat) {
  fdescTP new_fd;
  
  if ((new_fd = malloc(sizeof(fdescT))) == NULL) {
    printf("Not enough memory to open file.\n");
    return NULL;
  }
  //find an id that is not taken
  //so just look in the fd_table for the first not null entry and grab the index
  int i;
  for (i=0; i<MAX_FDS; i++) {
    if (fd_table[i] == NULL) {
      //we found an empty entry
      new_fd->id = i;
      fd_table[i] = new_fd; //store the new fdesc in the table
      break;
    }
  }
  if (i==MAX_FDS) {
    return NULL; //fd_table is full
  }
  
  //else ok
  new_fd->root_fat = rt_fat;
  new_fd->wr_blk = rt_fat;
  new_fd->wr_off = 0;
  new_fd->rd_blk = rt_fat;
  new_fd->rd_off = 0;
  
  ushort cur_blk = new_fd->wr_blk;
  fileTP assoc_file = NULL;//file associated with the file descriptor
  //we must position the write head at the end of the file so that we append when we write
  //first we find the file that corresponds to the file descriptor.
  for (i = 0; i<NUM_BLOCKS;i++) {
    if (root_dir[i] != NULL) {
      if (root_dir[i]->root_fat == rt_fat) {
	assoc_file = root_dir[i];
      }
    }
  }
  if (assoc_file == NULL) {
    printf("SFS_API You must create the file before creating the file descriptor\n");
    return NULL; //error
  }

  //we get the last block of the file
  while(FAT->next[cur_blk] != 0) {
    cur_blk = FAT->next[cur_blk];
  }
  new_fd->wr_blk = cur_blk;

  //now we must set the offset correctly
  int offset = assoc_file->size % BLOCK_SIZE;
  new_fd->wr_off = offset;
  
  return new_fd;
}

ushort extract_free_blk() {
  ushort emptyblk = FAT->next[NUM_BLOCKS];
  if (emptyblk == 0) return 0; //no more space
  FAT->next[NUM_BLOCKS] = FAT->next[emptyblk]; //update the head of the free block list
  FAT->next[emptyblk] = 0; //no next block
  //now we must update the changes to FAT on disk.
  safe_dWrite(0, &fat_block[0]);
  return emptyblk;
}
  
//this fucntion takes the name of a file and creates a new entry in the 
//in memory directory structure as well as on the directory block of the disk
//it returns the root fat where the file is located. It returns 0 on error
ushort add_file_to_dir(char* name) {
    //we need to find a free entry in the root_fat
    ushort emptyblk = extract_free_blk();
    if (emptyblk == 0) return 0; //no more space
    
    
    fileTP new_file;
    if ((new_file = malloc(sizeof(fileT))) == NULL) return 0; //not enough memory 
    
    strncpy(&(new_file->name[0]),name,FILENAME_SIZE -1);
    new_file->name[FILENAME_SIZE] = '\0';
    new_file->root_fat = emptyblk;
    new_file->size = 0;


    char fcb[FCB_SIZE];
    serialize_fileT(&(fcb[0]),new_file); //transform the fileT struct in new_file into a string
    memcpy(&(dir_block[emptyblk*FCB_SIZE]),&(fcb[0]),FCB_SIZE);//copy the fcb into the directory block
    
    if (safe_dWrite(1,&(dir_block[0]))== DISK_SEEK_ERROR) return 0;//update the dir block that is on disk. 
    
    //we store the new file inside the in memory directory structure at the same index as the fat root of the file.
    root_dir[emptyblk] = new_file;
    
    return emptyblk;//return the fat block
}


//opens file named name. returns the filedescriptor id on success
//returns -1 if there is no more space to create a new file in the directory
//returns -2 if too much files are opened
//return -3 if the file is already opened
int sfs_fopen(char* name) {

  char truncated_name[FILENAME_SIZE];
  strncpy(&truncated_name[0],name,25);
  truncated_name[FILENAME_SIZE -1] = '\0'; //null terminate in case name was longer than 25 chars
  
  //this returns the node in the list that has filename == trunc_name
  //or NULL if no such file was found
  int i;
  fileTP file_found;
  for(i=0; i < NUM_BLOCKS;i++) {
    file_found = root_dir[i];
    if (file_found) { //if not null
      if (strncmp(&(file_found->name[0]),truncated_name,FILENAME_SIZE) == 0) { //the filenames are equal
	//we found the file
	break;
      }
	
    }
    file_found = NULL;
  }
  
  if (file_found == NULL) { //no such file
    //we create the file
    ushort empty_fat_root;
    
    empty_fat_root = add_file_to_dir(truncated_name);
    if (empty_fat_root == 0) {
      return -1; //error, no more space
    }
    fdescTP new_fd = createFD(empty_fat_root); //create a new file descriptor
    if (new_fd == NULL) {
      //the file with size 0 will remain on disk. So the user should
      //close some files before opening this one.
      return -2; //fd table is full or we ran out of memory
    }

    return new_fd->id; //success return the filedesc id
  }
  else {
    //the file already exists
    ushort file_found_root = file_found->root_fat;
    ushort i = 0;
    int not_taken = -1;
    for (i = 0; i< MAX_FDS; i++){
      if (fd_table[i] != NULL) {
	//there is a fd at this location
	//we must check that the file is not already opened so
	//we look for a fd that has root_fat = file_found_root
	if (fd_table[i]->root_fat == file_found_root) {
	  //file already opened
	  return -3;
	}
      }
      else not_taken = i; //remember the index of a free fd table entry

     }
  
  
    //the file is not open since we cant find it in the fdesc table
    if (not_taken == -1) {
      //no more space in the fd table. Too many files are opened at the same time
      return -2;
    }
    else {
      //we create a fd to add to the table
      fdescTP new_fd;
      new_fd = createFD(file_found_root);
      return new_fd->id;//return the file decriptor id
    }

  }
}//end of open


//take the filedescriptor ID fid and close the file.
//that is, remove the fdesc from the fd_table
int sfs_fclose(int fid) {
  //fid corresponds to the index of the file descriptor table in which
  //the wanted file descriptor is stored.
  fdescTP wanted_fd = fd_table[fid];
  if (wanted_fd == NULL) return -1; //no such fid
  free(wanted_fd);
  int i = 0;
  fd_table[fid] = NULL;
  return 0; //success
}

//returns the number of bytes written.
//-1 if the fid cannot be found
//-2 if there is not enough space
int sfs_fwrite(int fid, char * buf, int length) {
  
  if (length <= 0) {
    return 0; //success, nothing to do
  }

  fdescTP fdesc = fd_table[fid];
  if (fdesc==NULL) {
    //no such fid
    return -1;
  }


  //we need to determine if we have enough space to store what the user wants to write
  int bytes_to_store = length - (BLOCK_SIZE - fdesc->wr_off);
  int num_blocks_required = bytes_to_store / BLOCK_SIZE; //integer division
  if ((bytes_to_store % BLOCK_SIZE) != 0) {
    num_blocks_required++;
  }

  //now we check if we have enough free blocks
  int count = 0;
  int cur_blk = FAT->next[NUM_BLOCKS]; //first free block
  while ( (count < num_blocks_required) && (cur_blk != 0)) {
    count ++;
    cur_blk = FAT->next[cur_blk];
  }

  if (count < num_blocks_required) {
    return -2; //not enough free blocks
  }

    
  //else ok

  //we must find the file structure corresponding to the passed in fid
  ushort file_root_fat = fd_table[fid % MAX_FDS]->root_fat; //we use modulo to make sure index is not too big
  fileTP the_file = root_dir[file_root_fat];
  //Now we are set for the writing.
  //the_file is the entry in the directory (name, size, root_fat)
  //fdesc is the file descriptor table entry
  
  int bytes_written = 0;
  char mem_block[BLOCK_SIZE]; //will contain the memory of the current block
  char * buf_pos = buf; //this will change as we write the buffer
  int b_in_block;
  int b_to_write_total;
  int b_to_write;//bytes to write inside the current block
  char fcb[FCB_SIZE];

  //read the new block into memory
  if (safe_dRead(fdesc->wr_blk,&(mem_block[0])) == DISK_SEEK_ERROR) return -1;

  while (bytes_written < length) { //loop until everything has been written

    if (fdesc->wr_off == BLOCK_SIZE) { 
      //we are at the end of a block so we have to grow the file
      
      //get a new block to store data
      ushort free_blk = extract_free_blk();
      FAT->next[fdesc->wr_blk] = free_blk;
      fdesc->wr_blk = free_blk; //next time we write, we will write on the next block
      fdesc->wr_off = 0;        //offset reset to 0

      //write back the modified fat
      if (safe_dWrite(0,&fat_block[0]) == DISK_SEEK_ERROR) return -1;
    
    }

    int b_in_block = BLOCK_SIZE - fdesc->wr_off; //bytes available to write in the block
    int b_to_write_total = length - bytes_written;
    if (b_to_write_total < b_in_block) { //cannot take all memory of this block
      b_to_write = b_to_write_total;
    }
    else {
      b_to_write = b_in_block;
    }
    //we must write b_to_write bytes in the current block
    memcpy(&(mem_block[0]) + fdesc->wr_off ,buf_pos,b_to_write);
    buf_pos = buf_pos + b_to_write;
    bytes_written = bytes_written + b_to_write;
    the_file->size = the_file->size + b_to_write;
    fdesc->wr_off = fdesc->wr_off + b_to_write;

    //we write back the current block back to the disk
    if (safe_dWrite(fdesc->wr_blk,&(mem_block[0])) == DISK_SEEK_ERROR) {
      //error on the real disk
      fdesc->wr_off = 0; //put the head back to the beginning of the block
      return -1;
    }

    //write back the modified file size
    serialize_fileT(&(fcb[0]),the_file);
    memcpy(&(dir_block[FCB_SIZE * the_file->root_fat]),&(fcb[0]),FCB_SIZE); //update the entry of the file in the in memory dir_block   
    if (safe_dWrite(1,&(dir_block[0])) == DISK_SEEK_ERROR) return -1; //write the directory block back to the real disk
  }//end while
  //files have been written
  return bytes_written;
} //end sfs_fwrite




//reads length bytes from the file identified by fid and puts them into buf
//it returns the number of bytes read or -1 on error.
int sfs_fread(int fid, char * buf, int length) {
  
  if (length <= 0) {
    return 0; //success, nothing to do
  }
  fdescTP fdesc = fd_table[fid];
  if (fdesc==NULL) {
    //no such fid
    return -1;
  }
  
  //else ok
  
  //we must find the file structure corresponding to the passed in fid
  ushort file_root_fat = fd_table[fid % MAX_FDS]->root_fat; //we use modulo to make sure index is not too big
  fileTP the_file = root_dir[file_root_fat];
 

  //we have to check how many bytes we are from the end of file
  int count = 0; //will count the number of blocks read so far
  ushort cur_blk = fdesc->root_fat; //the first block of the file
  while(cur_blk != fdesc->rd_blk) {
    cur_blk = FAT->next[cur_blk];
    count ++;
  }
  //now cur_blk points to the same block as the read head of the file
  int size_read_so_far = count * BLOCK_SIZE + fdesc->rd_off;

  if (length > (the_file->size - size_read_so_far)) {
    length = (the_file->size - size_read_so_far); //we cannot read past the end of file
  }

  //Now we are set for the reading
  //the_file is the entry in the directory (name, size, root_fat)
  //fdesc is the file descriptor table entry
  
  int bytes_read = 0;
  char mem_block[BLOCK_SIZE]; //will contain the memory of the current block
  char * buf_pos = buf; //this will change as we write the buffer
  
  if (safe_dRead(fdesc->rd_blk,&(mem_block[0])) == DISK_SEEK_ERROR) return -1;
  int b_in_block;
  int b_to_read_total;
  int b_to_read;//bytes to read inside the current block

  while (bytes_read < length) { //loop until everything has been written
    
    if (fdesc->rd_off == BLOCK_SIZE) { 
      //we are at the end of a block so we have to get the next block to continue reading      
      
      //get the next block to read data
      ushort nextblk = FAT->next[fdesc->rd_blk];
      if (nextblk == 0) {
	//we have reached end of file
	return bytes_read;
      }
      //else ok
      fdesc->rd_off = 0;        //offset reset to 0
      fdesc->rd_blk = nextblk;  //read the next block
      if (safe_dRead(fdesc->rd_blk,&(mem_block[0])) == DISK_SEEK_ERROR) return -1;
    }

    int b_in_block = BLOCK_SIZE - fdesc->rd_off; //bytes available to read in the block
    int b_to_read_total = length - bytes_read;

    //determine how many bytes we have to read in the current block
    if (b_to_read_total < b_in_block) { //cannot take all memory of this block
      b_to_read = b_to_read_total;
    }
    else {
      b_to_read = BLOCK_SIZE- fdesc->rd_off;
    }

    //we must read b_to_read bytes from the current block
    memcpy(buf_pos,&(mem_block[0]) + fdesc->rd_off,b_to_read); //copy the bytes
    buf_pos = buf_pos + b_to_read;
    bytes_read = bytes_read + b_to_read;
    fdesc->rd_off = fdesc->rd_off + b_to_read;    
  }//end while
  
  return bytes_read;
} //end sfs_fread


//removes a file from the filesystem
//if the file is currently open then we get an error
//returns 0 on success
//returns -1 if the file was not found in the directory
//returns -2 if the file is open
//returns -3 on disk error
int sfs_remove(char *name) {

  char truncated_name[FILENAME_SIZE];
  strncpy(&truncated_name[0],name,25);
  truncated_name[FILENAME_SIZE -1] = '\0'; //null terminate in case name was longer than 25 chars
  
  //this returns the node in the list that has filename == trunc_name
  //or NULL if no such file was found
  int i;
  fileTP file_found;
  for (i=0;i<NUM_BLOCKS;i++) {
    file_found = root_dir[i];
    if (file_found) { //if not null
      if (strncmp(&(file_found->name[0]),truncated_name,FILENAME_SIZE) == 0) { //the filenames are equal
	//we found the file
	break;
      }
	
    }
    file_found = NULL;
  }
  
  if (file_found == NULL) { //no such file
    return -1;
  }

  //else we found the file
  //we look if the file is open : if it is open, then there is an entry in the fd_table that corresponds to it

 
  fdescTP fdesc = NULL;
  
  for (i=0; i<MAX_FDS;i++) { 
    
    fdesc = fd_table[i];
    if (fdesc) { //fdesc not null
     
      if (fdesc->root_fat == file_found->root_fat) { //bingo! we know its open
	
	return -2;
      }
    }
  } //end for loop

  //the file is not open, we can delete it
  //we need to find the last data block of the file
  //so that we can link the blocks of the file on the free block list
  ushort cur_blk = file_found->root_fat;
  while (FAT->next[cur_blk] != 0) {
    cur_blk = FAT->next[cur_blk];
  }
  
  //cur_blk is now the last block of the file
  
  FAT->next[cur_blk] = FAT->next[NUM_BLOCKS];
  FAT->next[NUM_BLOCKS] = cur_blk;

  //write the fat to the virtual disk
  if (safe_dWrite(0, &(fat_block[0])) == DISK_SEEK_ERROR) {
    return -3;
  }

  //now we must delete the file from the root directory
  //we just put a null character on the first byte of the FCB corresponding to the file
  dir_block[FCB_SIZE * file_found->root_fat] = '\0';

  //write the directory block back onto the disk
  if (safe_dWrite(1, &(dir_block[0])) == DISK_SEEK_ERROR) {
    return -3;
  }
  
  //now delete the fileT structure from the in_memory directory structure
  root_dir[file_found->root_fat] = NULL;
  free(file_found);

  return 0;
}
  

//remove every in memory data structure, free any memory that was allocated and close
//the filehandle of DiskSimulate.dat
int sfs_clean() {
  //destroy the filedescriptor table
  int i;
  for (i= 0 ; i < MAX_FDS ; i++ ) {
    if (fd_table[i]) { //not null entry
      free(fd_table[i]);
      fd_table[i] = NULL;
    }
  }
  
  for (i=0; i < NUM_BLOCKS ; i++) {
    if (root_dir[i]) { //not null entry
      free(root_dir[i]);
      root_dir[i] = NULL;
    }
  }
  
  free(FAT);

  if (finishDisk() == DISK_SEEK_ERROR) {
    return -1;
  }
  
  return 0; //success
}
