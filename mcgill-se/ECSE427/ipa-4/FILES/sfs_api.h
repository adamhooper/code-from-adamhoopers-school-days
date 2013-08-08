#ifndef SFS_API_H
#define SFS_API_H


/* 
   SIMPLE FILE SYSTEM HEADER FILE
   IMPLEMENTATION IN SFS_API.C
*/


#define TRUE 1
#define FALSE 0
#define FILENAME_SIZE 26


//max number of files opened at any given time
#define MAX_FDS 16

//size of the file control block
#define FCB_SIZE 32

//in memory structure for FAT
//the fat only consists in one column of next pointers
//if you want the mem block address after block 5 then it is next[5]
//(the index of the fat corresponding to a datablock is the same as the the data_block address)
typedef struct fat {
  ushort* next;
} fatT, * fatTP;

//in memory structure for files
//should be 32 bytes if packed (FCB_SIZE)
typedef struct file {
  char name[FILENAME_SIZE];	//filename 25 letters max
  ushort root_fat;
  int size;//size of the file
} fileT, *fileTP;


//structure for file descriptors
typedef struct fdesc {
	int id;
	ushort root_fat;
	ushort wr_blk;	//block number under the write ptr
	ushort wr_off;	//write offset
	ushort rd_blk; 	//block number under the read ptr
	ushort rd_off;	//read offset from the start of the block
} fdescT, *fdescTP;


//this function
//takes a fileT structure and serializes it into a string of bytes
char* serialize_fileT(char* dest, fileTP source);

//this is the inverse operation of function serialize_fileT
fileTP unserialize_fileT(fileTP dest, char* source);

// This function calls dRead until success
// It catches a DISK_SEEK_ERROR if it happens and returns it
int safe_dRead(int addr,char*buf);

// This function calls dWrite until success
// it catches a DISK_SEEK_ERROR if it happens and returns it
int safe_dWrite(int addr, char *buf);


//this function reads the contents of block 1 and constructs
//the global structure root_dir that contains the fileT structs.
//The byte array read from disk is cached in memory into the variable
//dir_block[BLOCK_SIZE]
int read_root_dir();

//this function initializes the in memory directory structure by cleaning/setting to
//NULL all the file entries in the in memory directory structure root_dir
int init_dir();

//this function simply reads block 0 of the disk into the global char array
//fat_block
int read_fat_to_mem();

//This function writes in the last entry of the fat the next free block. And then
//for the next free block sets the pointer to the next free block... until it reaches the
//last free block. The next block of the last free block is block 0, which is used as a sentinel. 
//this routine needs to be run on a fresh virtual disk otherwise, it screws everything
int create_free_block_chain();

//This creates a new filedescriptor set for the file that starts
//at root fat rt_fat. It is used by sfs_fopen
//the filedescriptor is inscribed in the in memory filedescriptor table
fdescTP createFD(ushort rt_fat);

//This function gets a free block out of the fat. the free block is allocated and
//the fat is rewritten on disk
ushort extract_free_blk();


//this fucntion takes the name of a file and creates a new entry in the 
//in memory directory structure as well as on the directory block of the disk
//it returns the root fat where the file is located. It returns 0 on error
//the size of the file is initialized to 0
//if there is not enough memory, the function returns 0
ushort add_file_to_dir(char* name);



/*------API FUNCTIONS-------------------------------------*/

//creates the filesystem. Everything is erased if fresh is set to TRUE
//otherwise, the information is loaded from the disk file.
//returns 0 on succcess
//returns -1 on error
int  mksfs(int fresh);

//prints the contents of the directory of the virtual disk
void sfs_ls();

//opens file named name. returns the filedescriptor id on success
//returns -1 if there is no more space to create a new file in the directory
//returns -2 if too much files are opened
//return -3 if the file is already opened
int sfs_fopen(char* name);

//take the filedescriptor ID fid and close the file.
//that is, remove the fdesc from the fd_table
//returns 0 on success, -1 if there is no such file descriptor
int sfs_fclose(int fid);

//writes length bytes from buf to file descriptor fid
//returns the number of bytes written.
//or -1 if the fid cannot be found
//or -2 if there is not enough space on the disk
int sfs_fwrite(int fid, char * buf, int length);

//reads length bytes from the file identified by fid and puts them into buf
//it returns the number of bytes read or -1 on error.
int sfs_fread(int fid, char * buf, int length);

//removes a file from the filesystem
//if the file is currently open then we get an error
//returns 0 on success
//returns -1 if the file was not found in the directory
//returns -2 if the file is open
//returns -3 on disk error
int sfs_remove(char *name);

//this function closes all the data structures, frees the memory
//and closes the file handle to the real file DiskSimulate.dat
int sfs_clean();

#endif
