#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_SZ 1000

int main(int argc, const char **argv) 
{

	printf(1,"%d\n",uptime());

	pause(1, 100);
	sleep(100);
	printf(1,"%d\n",uptime());

	pause(1, 10);
	wait();
	sleep(10);

	printf(1,"%d\n",uptime());

	pause(1, 50);
	sleep(50);

	printf(1,"%d\n",uptime());

	exit();

}
