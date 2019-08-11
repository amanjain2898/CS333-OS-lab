#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    pid_t x = fork();
    if(x == 0){
    	pid_t a = getpid(),b=getppid();
    	printf("Child : My process ID is: %d\n",a);
    	printf("Child : My parent process ID is: %d\n",b);
    }
    else{

        wait(NULL);
        pid_t b=getpid();
        printf("Parent : My process ID is: %d\n",b);
        printf("Parent : My child process ID is: %d\n",x);
        printf("Parent : The child with process ID %d has terminated\n",x);
    }
    return 0;
}