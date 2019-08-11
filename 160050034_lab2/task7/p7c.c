#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(int argc,char* argv[]){
	int n = atoi(argv[1]);
    int m=n;
	printf("Parent is : %d\n",getpid());
	printf("Number of children: %d\n",n);
    int arr[n];
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
            exit (0);
        }
        else{
            arr[m-n] = x;
            n--;
            printf("Child %d is created\n",x);
            continue;
        }
    }
    // for(int i=n-1;i>=0;i--){
    //     printf("%d ",arr[i]);
    // }
    // printf("\n");
    for(int i=m-1;i>=0;i--){
        int stat;
        waitpid(arr[i],&stat,0);
        printf("Child %d exited\n",arr[i]);
    }
    printf("Parent exited\n");
    return 0;


    // for(int i=0;i<n;i++){
	   //  wait(NULL);
    // }
}

