#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void handler(int sig)
{
  pid_t pid;
  pid = wait(NULL);
  printf("Parent %d: Child %d is exited.\n",getpid(), pid);
}

int main(){
    signal(SIGCHLD, handler);

    int sleep1 = 9;
    int sleep2 = 4;
	
    pid_t x = fork();

    if(x == 0){
        pid_t y = fork();
        if(y == 0){
            printf("Child %d is created by parent %d (sleeps for %d seconds)\n",getpid(),getppid(),sleep1);
            sleep(sleep1);
            exit(0);
        }
        else{
            sleep(sleep2);
        }
        exit(0);
    }
    else{
        printf("Child %d is created by parent %d (sleeps for %d seconds)\n",x,getpid(),sleep2);
    }
    while(1) sleep(1);
    return 0;
}