#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>


int main(int argc,char* argv[]){

	if(fork() == 0){
		close(0);
		int fd = open(argv[1],O_RDONLY);
		char *args[]={"./mycat",NULL};
	    execvp(args[0],args);
	}
	else{
		wait(NULL);
		printf("Parent: The child has terminated\n");
	    // write(fp,"Parent : The child has terminated\n",34);
	}
}