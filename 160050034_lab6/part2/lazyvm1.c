#include "types.h"
#include "stat.h"
#include "user.h"

#define KERNBASE 0x80000000 

#define ARRAY_SIZE 2048
int buf[ARRAY_SIZE];

#define EXTRA_SPACE 8182

int getpusz()
{
	/*todo*/
	int ans=0;
	int flags;
	uint pa;
	for(uint i=0;i<KERNBASE;i=i+4096){
		if(get_va_to_pa(i, &pa, &flags)){
			ans += 4096;
		}
	}
	return ans;
}

void print_status()
{
	int vp;
	int pp;
	int sz;

	sz = getpsz();
	vp = sz / 4096;
	if((sz % 4096) != 0)
		vp += 1;
	pp = getpusz() / 4096;

	printf(1, "sz: %d, vp: %d, pp: %d\n", sz, vp, pp);
}


int
main(int argc, char *argv[])
{	
	buf[ARRAY_SIZE-1] = 1234;

	print_status();

	int * ptr = (int *)&buf[ARRAY_SIZE-1];
	printf(1, "addr: %d, value: %d\n", ((int)ptr), *ptr);

	sbrk(EXTRA_SPACE);

	print_status();
	
	ptr = (int *)20484;
	printf(1, "addr: %d, value: %d\n", ((int)ptr), *ptr);

	print_status();
	
	ptr = (int *)28658;
	printf(1, "addr: %d, value: %d\n", ((int)ptr), *ptr);

	print_status();

	ptr = (int *)28676;
	printf(1, "addr: %d\n", ((int)ptr));
	printf(1, "value: %d\n", *ptr);

	print_status();
		
	exit();
}