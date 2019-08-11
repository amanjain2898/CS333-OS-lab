#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>


int main(int argc,char* argv[]){
	int fp = open(argv[1],O_WRONLY);

	if(fork() == 0){
		pid_t a = getpid();
		printf("Child %d: writing to file %d\n",a,fp);
		write(fp,"Child : hello world! I am the Child\n",36);
	}
	else{
		pid_t a = getpid();
		printf("Parent: File opened. fd=%d\n",fp);
		printf("Parent %d: writing to file %d\n",a,fp);
		wait(NULL);
		write(fp,"Parent : hello world! I am the parent\n",38);
	    printf("Parent : The child has terminated\n");
	}
}