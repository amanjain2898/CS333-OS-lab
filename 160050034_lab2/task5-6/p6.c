#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/wait.h>

int main(){
  pid_t x = fork();
    if(x == 0){
      pid_t a = getpid(),b=getppid();
      printf("Child : My process ID is: %d\n",a);
      printf("Child : The parent process ID is: %d\n",b);
      getchar();
      exit (0);
      
    }
    else{
        pid_t b=getpid();
        printf("Parent : My process ID is: %d\n",b);
        printf("Parent : The child process ID is: %d\n",x);
        sleep(60);
        wait(NULL);
        printf("Parent : The parent has terminated");
    }
    return 0;
}