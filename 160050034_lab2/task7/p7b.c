#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(int argc,char* argv[]){
	int n = atoi(argv[1]);
    int n1=n;
	printf("Parent is : %d\n",getpid());
	printf("Number of children: %d\n",n);
    unsigned int ss = time(NULL);
	while(n>0)
    {
        int r = (rand_r(&ss)%50)/5;
    	pid_t x = fork();
        if(x == 0)
        {
        	// pid_t x1 = getpid();
         //    srand(x1);
         //    int a = (rand()/1000);
            // printf("%d\n",a);
            // printf("%d\n",a);
            // usleep(a);
            sleep(r+1);
            printf("Child %d exited\n",getpid());
            exit (0);
        }
        else{
            n--;
            printf("Child %d is created \n",x);
        	continue;
        }
    }
    for(int i=0;i<n1;i++){
	    wait(NULL);
    }
    printf("Parent exited\n");
    return 0;
    // for(int i=0;i<n;i++){
	   //  wait(NULL);
    // }
}

