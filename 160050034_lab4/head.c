#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
cat(int fd,int nlines)
{
  int n;
  int count=0;
  while((n = read(fd, buf, sizeof(buf))) > 0 && count < nlines) {
    // if (write(1, buf, n) != n) {
    //   printf(1, "cat: write error\n");
    //   exit();
    // }
    for(int i=0;i<n;i++){
      if(count > nlines) break;
      if(buf[i] == '\n')count++;
      write(1,buf+i,1);
    }
  }
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    cat(0,0);
    exit();
  }

  int nlines = atoi(argv[1])-1;

  for(i = 2; i < argc; i++){
    printf(1,"--- %s ---\n",argv[i]);
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
    cat(fd,nlines);
    close(fd);
  }
  exit();
}
