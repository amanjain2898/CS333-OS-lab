#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/wait.h>

int main(){
	pid_t x = fork();
    if(x == 0){
    	pid_t a = getpid(),b=getppid();
    	printf("Child : My process ID is: %d\n",a);
    	printf("Child : The parent process ID is: %d\n",b);
    	sleep(5);
    	b=getppid();
    	printf("Child : My process ID is: %d\n",a);
    	printf("Child : The parent process ID is: %d\n",b);
    }
    else{
    	printf("Parent : My process ID is: %d\n",getpid());
        printf("Parent : The child process ID is: %d\n",x);
        sleep(3);
        pid_t b=getpid();
        printf("Parent : The parent has terminated\n");
    }
    return 0;
}