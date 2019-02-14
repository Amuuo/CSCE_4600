#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <cstring>
#include <mutex>
#include <fstream>
#include <iostream>

using namespace std;
mutex my_mutex;
char *buff;

fstream F;
int semaphore;

int main() {
  semaphore = shm_open("sema", O_RDWR | O_CREAT, S_IRWXO | S_IRWXG | S_IRWXU);
  pid_t parent_pid;
  parent_pid = getpid();
  
  caddr_t page_address;

  F.open("fileF", ios::out | ios::trunc);
  F << 1;
  F.close();

  for (int i = 0; i < 3; ++i) {
    if (fork() == 0) {
      cout << "\nChild PID: " << getpid() << endl;
      page_address = (caddr_t)mmap(0, 5000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, semaphore, 0);
      break;
    }
  }
  
  if (getpid() != parent_pid) {
    int N = 0;
    while (N < 500) {      
      if (mlock(page_address, 5000) == 0) {        
        F.open("fileF", ios::in);
        F.seekp(0);
        F >> N;
        F.close();
        cout << "\nPID: " << getpid() << ", N: " << N << endl;
        N++;
        F.open("fileF", ios::out);
        F.seekp(0);
        F << N;
        F.close();
        munmap(page_address, 5000);
      }
    }
    exit(0);
  }
  
  return 0;
}