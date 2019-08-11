#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void handler(int sig)
{
  pid_t pid;
  pid = wait(NULL);
  printf("Child %d is exited.\n", pid);
}

int main(int argc,char* argv[]){
	int n = atoi(argv[1]);
    int m=n;
	// printf("Parent is : %d\n",getpid());
	// printf("Number of children: %d\n",n);
    int arr[n];
    unsigned int ss = time(NULL);

    signal(SIGCHLD, handler);
	while(n>0)
    {
        int r = (rand_r(&ss)%100)/5;
    	pid_t x = fork();
        if(x == 0)
        {  
            sleep(r+1);
            exit (0);
        }
        else{
            arr[m-n] = x;
            n--;
            printf("Child %d is created (sleeps for %d seconds)\n",x,r+1);
            continue;
        }
    }
    while(1) sleep(1);
    // printf("Parent exited\n");
    return 0;
}