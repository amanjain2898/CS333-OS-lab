#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>

void rec_fork(pid_t x,int n){
	if(x > 0){
		wait(NULL);
		printf("Child %d exited\n",x);
		return;
	}
	if(n==0){
		pid_t a = getpid();
		printf("Child %d is created\n",a);
		// exit (0);
		return;
	}
	if(x==0){
		pid_t a = getpid();
		printf("Child %d is created\n",a);
		pid_t b = fork();
		rec_fork(b,n-1);
		return;
	}
}


int main(int argc,char* argv[]){
	int n = atoi(argv[1]);
	printf("Parent is : %d\n",getpid());
	printf("Number of children: %d\n",n);
	pid_t x = fork();
	rec_fork(x,n-1);
	wait(NULL);
	if(x!=0) printf("Parent exited\n");
    return 0;
}

