#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;


  // modified code

  // int addr;
  // int n;
  // if(argint(0, &n) < 0)
  //   return -1;
  // addr = myproc()->sz;
  // myproc()->sz += n;
  // return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int sys_get_va_to_pa(void)
{
  uint va;
  uint* pa;
  int* flag;

  if(argint(0,(int*) &va) < 0)
    return -1;
  if(argptr(1,(char**)&pa,32) < 0)
    return -1;
  if(argptr(2,(char**)&flag,32) < 0)
    return -1;


  pde_t *pde;
  uint* temp;
  pte_t *pgtab;

  pde = &(myproc()->pgdir)[PDX(va)];
  if(*pde & PTE_P){
    pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
  } else {
      return 0;
  }
  temp = (uint*)(&pgtab[PTX(va)]);

  if(temp == 0 || !(*temp & PTE_P))
      return 0;
  else {
      *pa = PTE_ADDR(*temp) | (va & 0xFFF);
      return 1;
  }

return 1;
}

int sys_getpsz(void){
  return myproc()->sz;
}