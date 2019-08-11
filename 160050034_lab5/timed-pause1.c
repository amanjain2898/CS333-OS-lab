#include "types.h"
#include "stat.h"
#include "user.h"

// Pausing the process itself

#define MAX_SZ 1000

int main(int argc, const char **argv) 
{
	printf(1,"%d\n",uptime());

	pause(getpid(), 500);

	sleep(2);

	printf(1,"%d\n",uptime());
	exit();

}
