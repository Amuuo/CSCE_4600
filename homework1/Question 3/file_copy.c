/*
*============================================================================
* Name        : Homework #1 : Question #3
* Author      : Adam Williams
* Version     : 1.0
* Copyright   : 2018
* Description : Program opens fileA and fileB and copies contents from
                fileA to fileB argv[2] bytes at a time
*============================================================================
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

long filesize;
long filestep;
int i;


int main(int argc, char** argv) {

  filesize = atol(argv[1]);
  filestep = atol(argv[2]);

  char buffer[filestep];

  int fileA = open("/tmp/fileA", O_RDONLY);
  int fileB = open("/tmp/fileB", O_WRONLY | O_APPEND);

  for(i = 0; i < filesize; i+=filestep){
      read(fileA, buffer, filestep);
      write(fileB, buffer, filestep);
  }
  close(fileA);
  close(fileB);
}