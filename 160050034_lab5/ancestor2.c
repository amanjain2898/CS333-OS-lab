#include "types.h"
#include "stat.h"
#include "user.h"

// What happens when the grandchild is the last one to terminate and the child gets terminated first

int main() 
{
	int ret = fork();
	if (ret == 0) 
	{
		int gd = fork();
		if(gd == 0){
			printf(1, "\ngd: pid %d\n", getpid());
			printf(1, "gd: parent pid %d\n", getppid());

			sleep(100);

			printf(1, "\ngd: pid %d\n", getpid());
			printf(1, "gd: parent pid %d\n", getppid());

			exit();
		}

		printf(1, "\nchild: pid %d\n", getpid());
		printf(1, "child: parent pid %d\n", getppid());

		sleep(200);

		printf(1, "\nchild: pid %d\n", getpid());
		printf(1, "child: parent pid %d\n", getppid());

		exit();
	}
	else
	{
		sleep(50);

		printf(1, "\nparent: pid %d\n", getpid());
		printf(1, "parent: parent pid %d \n", getppid());
		printf(1, "parent: child pid %d\n", ret);
	}
	
	exit();
}