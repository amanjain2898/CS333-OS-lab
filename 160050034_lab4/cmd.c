#include "types.h"
#include "stat.h"
#include "user.h"


int
main(int argc, char *argv[])
{

  if(argc <= 1){
    exit();
  }

  int pid = fork();
  if(pid>0){
    wait();
  }
  else if(pid==0){
    exec(argv[1],argv+1);
  }
  else{
    printf(1,"fork error\n");
  }
  exit();
}
