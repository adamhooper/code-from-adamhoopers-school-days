#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "sfs_api.h"

/* 
Simple file system test program
*/

int main(void) {
  int i;
  char as[BLOCK_SIZE + 23];
  char bs[BLOCK_SIZE + 567];

  //initialize a fresh disk
  if (mksfs(1)) {
    printf("Error in initializing the disk\n");
    exit(1);
  }
  
  printf("Currently no files have been created.\n");
  printf("We run ls() : \n");

  sfs_ls();

  // some data to write
  for (i=0; i<BLOCK_SIZE + 23; i++) {
    as[i] = 'a';
  }

  for (i=0; i<BLOCK_SIZE + 567; i++) {
    bs[i] = 'b';
  }


  int file1 = sfs_fopen("File1");
  int file2 = sfs_fopen("File2");
  int file3 = sfs_fopen("big_file_number3.txt");
  
  printf("Now, 3 files have been created.\n\n");
  printf("The file descriptor ids are : %d %d %d\n",file1,file2,file3);
  printf("We run ls():\n\n");

  sfs_ls();
  
  printf("\n Now we will write %d a's into the second file\n",BLOCK_SIZE + 23);
  int wr_result = sfs_fwrite(file2, &as[0],BLOCK_SIZE + 23);
  
  printf("Result : %d bytes written\n", wr_result);

  printf("And we will write %d b's into the 1st file\n",BLOCK_SIZE + 567);
  wr_result = sfs_fwrite(file1, &bs[0],BLOCK_SIZE + 567);
  
  printf("Result : %d bytes written\n", wr_result);
  
  printf("And now, ls() : \n\n");

  sfs_ls();

  printf("Now, I will close file 'File1'. sfs_fclose returns -1 on error and 0 on success\n");
  
  int fclosereturn = sfs_fclose(file1);
  if (fclosereturn) {
    printf("Invalid filedescriptor\n");
  }
  else { printf("Success!\n");}
  
  printf("We do ls() : \n\n");
  sfs_ls();
  

  printf("Now we open the file that we just closed.\n");
  file1 = sfs_fopen("File1");
  
  printf("The new file descriptor is %d \n",file1);

  printf("Now we close and then remove File2 from the disk\n");
  
  fclosereturn = sfs_fclose(file2);

  if (fclosereturn) {
    printf("Invalid filedescriptor\n");
  }
  else { printf("Success!\n");}

  int removeres = sfs_remove("File2");

  if (removeres) {
    printf("%d Error in removal",removeres);
  }
  else { printf("Success! file was removed\n");}

  printf("\nNow we do ls() : \n");
  
  sfs_ls();
  
  char scrap_book[2*BLOCK_SIZE];

  
  printf("Now it's time to read stuff that was copied inside File1\n");
  printf("First let's read 1000 bytes, then 1200, and then 600\n");
  printf("sfs_fread return the number of bytes that were read.\n");
  
  int pos;
  int read_res = sfs_fread(file1,&scrap_book[0],1000);

  printf("%d bytes read.\n",read_res);

  pos = pos + read_res;
  
  read_res = sfs_fread(file1, &scrap_book[pos],1200);
  
  printf("%d bytes read.\n",read_res);

  pos = pos + read_res;
  
  read_res = sfs_fread(file1, &scrap_book[pos],600);
  
  printf("%d bytes read.\n",read_res);

  pos = pos + read_res;

  scrap_book[pos] = '\0'; //null terminate so we can print it

  printf("What we have read: \n%s\n",&scrap_book[0]);

  printf("It doesn't read past EOF.\n");
  
  printf("We will now write what we have read into big_file_number3.txt\n");

  sfs_fwrite(file3, &scrap_book[0], pos);

  printf("And now, we ls() : \n");

  sfs_ls();


  printf("Now we reset the filesystem\n");

  sfs_clean();
  
  printf("FileSystem Reset\n");

  printf("We want to reload the disk\n");
  
  mksfs(0);
  
  printf("Filesystem reloaded. We do ls \n");
  
  sfs_ls();

  int open_result = sfs_fopen("File1");
  if (open_result >= 0) {
    printf("File File1 opened with success, fdesc id: %d\n",open_result);
  }
  char test5[80];
  for (i=0;i<80;i++) {
    test5[i] = '0';
  }
  wr_result = sfs_fwrite(open_result, &test5[0], 79);

  printf("%d bytes written to file File1.\n",wr_result);
  
  sfs_ls();
  
  read_res = sfs_fread(open_result, &scrap_book[0], 2*BLOCK_SIZE -2);
  scrap_book[read_res] = '\0';

  printf("Print what we have read : %d bytes\n",read_res);

  printf("%s\n",&scrap_book[0]);



}
  
