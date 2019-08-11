#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>

int main(){
	int counter = 0;
	pid_t pid;
    if(fork() == 0){
    	pid_t a = getpid(),b=getppid();
    	printf("Parent : My process ID is: %d\n",b);
    	printf("Parent : The child process ID is: %d\n",a);

    	printf("Child : My process ID is: %d\n",a);
    	printf("Child : The parent process ID is: %d\n",b);
    }
    return 0;
}