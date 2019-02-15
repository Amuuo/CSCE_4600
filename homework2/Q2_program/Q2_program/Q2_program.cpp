/*
*============================================================================
* Name        : Homework #2 : Question #2
* Author      : Adam Williams
* Version     : 1.0
* Copyright   : 2018
* Description : Program initializes 3 processes via fork(), and utilizes a
                semaphore to synchronize read/writes to file F from 1 to 500
*============================================================================
*/
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;


sem_t* semaphore;
int shared_addr;
fstream F;
char pid_name[20];
void increment_counter(int*);
void print_header();
string colors[3] = { "[32m", "[33m", "[34m" };

//====================================
//              M A I N
//====================================

int main() {
  
  pid_t pid;
  int status{0};
  print_header();

  // open shared memory space
  if ((shared_addr = shm_open("shared_test", O_CREAT | O_RDWR, S_IRWXU)) == -1) {
    cout << "\nShared filed failed to init" << endl;
    exit(1);
  }

  // open memory space for semaphore
  if ((semaphore = sem_open("semaphore_test", O_CREAT | O_TRUNC, S_IRWXU, 1)) 
                                                                == SEM_FAILED) {
    cout << "\nSemaphore failed to init" << endl;
    exit(1);
  }

  // allocate memory to memory space
  fallocate(shared_addr, 0, 0, 1000);
  ftruncate(shared_addr, 1000);

  // assign N memory from shared memory space and set to 0
  int* N = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE | PROT_EXEC, 
                 MAP_SHARED, shared_addr, 0);
  *N = 0;


  // open file F and insert 1    
  F.open("/tmp/fileF", ios::out | ios::trunc);
  F << 1;
  F.close();

  // create 3 child processes with fork() and start incrementing file
  for (int i = 0; i < 3; ++i) {
    if ((pid = fork()) == 0) {
      strcat(pid_name, colors[i].c_str());
      strcat(pid_name, to_string(getpid()).c_str());
      strcat(pid_name, "[0m");      
      increment_counter(N);
    }
  }

  // if parent_process, wait for children to exit, then unlink shared memory
  if (pid != 0) {
    while (wait(&status) > 0);
    shm_unlink("shared_test");
    sem_close(semaphore);
    sem_unlink("semaphore_test");
  }

  return 0;
}




// function uses semaphore block access to other process while
// reading value from file, assigning the value to N, incrementing,
// and writing the incremented value back to F

void increment_counter(int* N) {
  pid_t child_pid = getpid();
  
  while (*N < 500) {
    sem_wait(semaphore);
    F.open("/tmp/fileF", ios::in);
    F >> *N;
    F.close();
    cout << "\tPID: " << pid_name << ", N: " << *N << endl;
    ++(*N);
    F.open("/tmp/fileF", ios::out | ios::trunc);
    F << *N;
    F.close();
    sem_post(semaphore);
    usleep(1);
  }
  exit(0);
}





void print_header()
{
  printf("\n\n"
    "       [37;42m +------------------------------------------------+ [0m\n"
    "       [37;42m |        Computer Sceince and Engineering        | [0m\n"
    "       [37;42m |          CSCE 4600 - Operating Systems         | [0m\n"
    "       [37;42m | Adam Williams arw0174 adamwilliams2@my.unt.edu | [0m\n"
    "       [37;42m +------------------------------------------------+ [0m\n");
}
