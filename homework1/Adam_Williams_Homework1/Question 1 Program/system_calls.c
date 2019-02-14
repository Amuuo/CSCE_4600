/*
*============================================================================
* Name        : Homework #1 : Question #1
* Author      : Adam Williams
* Version     : 1.0
* Copyright   : 2018
* Description : Program demostrates the following unix system calls:
*
*                - fork()
*                - wait()
*                - getpid()
*                - getppid()
*                - exit ()
*                - execl ()
*============================================================================
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const char* title = "\n\n"
"       [37;42m +------------------------------------------------+ [0m\n"
"       [37;42m |        Computer Sceince and Engineering        | [0m\n"
"       [37;42m |          CSCE 4600 - Operating Systems         | [0m\n"
"       [37;42m | Adam Williams arw0174 adamwilliams2@my.unt.edu | [0m\n"
"       [37;42m +------------------------------------------------+ [0m\n";

int main(){

    system("clear");
    printf("%s", title);

    printf("\n[33m>> Process will fork and then wait for child to exit..[0m");
    printf("\n[33m>> Child process will implement getpid(), getppid(), and exit()[0m");
    printf("\n[33m>> Parent process will implement wait(), getpid(), and execl()[0m");

    printf("\n\n\n\tForking process...");
    fflush(stdout);

    /* fork main process */
    if (fork() == 0){
        printf("\n\n\t[35mCHILD PROCESS:[0m\n");
        printf("\n\t\tPID: %d", getpid());
        printf("\n\t\tParent PID: %d", getppid());
        printf("\n\n\t\tExiting forked process...\n");
        exit(0);
    }
    /* run parent process */
    else{
        wait();
        printf("\n\n\t[35mPARENT PROCESS:[0m\n");
        printf("\n\t\tDone waiting for child process");
        printf("\n\t\tPID: %d", getpid());
        printf("\n\n\t\tExecuting 'date' with execl:\n\n\n");
        fflush(stdout);
        execl("/bin/date", "date", NULL);
    }
}
