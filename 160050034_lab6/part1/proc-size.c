#include "types.h"
#include "stat.h"
#include "user.h"

#define KERNBASE 0x80000000 
#define PHYSTOP   0xE000000 

char b[1024];


int getpusz()
{
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
int getpksz()
{
	int ans=0;
	int flags;
	uint pa;
	for(uint i=KERNBASE;i<0xFFFFFFFF;i=i+4096){
		if(i==0) break;
		if(get_va_to_pa(i, &pa, &flags)){
			ans += 4096;
		}
	}
	return ans;
}

int main(int argc, char *argv[])
{
	char *buf;

	printf(1, "\ngetpsz: %d bytes \n", getpsz());
	printf(1,"getpusz: %d bytes \n",getpusz());
	printf(1,"getpksz: %d bytes\n",getpksz());


	buf=sbrk(4096);
	buf[0]='\0';
	printf(1, "\ngetpsz: %d bytes \n", getpsz());
	printf(1,"getpusz: %d bytes \n",getpusz());
	printf(1,"getpksz: %d bytes\n",getpksz());

	
	buf=sbrk(4096*7);
	printf(1, "\ngetpsz: %d bytes \n", getpsz());
	printf(1,"getpusz: %d bytes \n",getpusz());
	printf(1,"getpksz: %d bytes\n",getpksz());

	buf=sbrk(1);
	printf(1, "\ngetpsz: %d bytes \n", getpsz());
	printf(1,"getpusz: %d bytes \n",getpusz());
	printf(1,"getpksz: %d bytes\n",getpksz());

	buf=sbrk(2);
	printf(1, "\ngetpsz: %d bytes \n", getpsz());
	printf(1,"getpusz: %d bytes \n",getpusz());
	printf(1,"getpksz: %d bytes\n",getpksz());


	exit();
}