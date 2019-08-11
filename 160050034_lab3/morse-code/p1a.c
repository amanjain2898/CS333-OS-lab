#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void  INThandler(int sig)
{
    if(sig==SIGINT){
         char  c;
         signal(sig, SIG_IGN);
         printf("Interrupt SIGINT received.\n");
         printf("Do you really want to quit (y|n)?\n");
         c = getchar();
         if (c == 'y' || c == 'Y')
              exit(0);
         else
              signal(SIGINT, INThandler);
         getchar(); 
     }
     else if(sig == SIGTERM){
         char  c;
         printf("Interrupt SIGTERM received.\n");
         printf("Do you really want to quit (y|n)?\n");
         c = getchar();
         if (c == 'y' || c == 'Y')
              exit(0);
         else
              signal(SIGINT, INThandler);
         getchar(); 
     }
}


 
int main ()
{
    // printf("pid: %d\n",getpid());
    if(signal(SIGINT, INThandler) == SIG_ERR){
        perror("Error: cannot handle SIGINT");
    }
    if(signal(SIGTERM, INThandler) == SIG_ERR){
        perror("Error: cannot handle SIGTERM");
    }
     while (1)
          pause();
     return 0;
}