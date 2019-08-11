#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
	if(fork() == 0){
		char *args[]={"./mycat",NULL};
	    execvp(args[0],args);
	}
	else{
		wait(NULL);
	    printf("Parent : The child process is terminated");
	}
}