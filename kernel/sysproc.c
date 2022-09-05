#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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
}

uint64
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


#ifdef LAB_PGTBL
#define MAX_SCANNED_NUM (64)
int
sys_pgaccess(void)
{
  uint64 va, i;
  int pagenum;
  uint64 userbuf;
  uint flag = ~PTE_A;
  uint64 bitmask = 0;
  pagetable_t pgtbl = myproc()->pagetable;

  // lab pgtbl: your code here.
  if(argaddr(0, &va) < 0)
    return -1;

  if(argint(1, &pagenum) < 0)
    return -1;

  if(argaddr(2, &userbuf) < 0)
    return -1;

  if(pagenum > MAX_SCANNED_NUM) {
    return -1;
  }

  for(i = 0; i < pagenum; i++) {
    pte_t *pte;
    if((pte = walk(pgtbl, va + i * PGSIZE, 0)) == 0) {
      continue;
    }
    if((*pte & PTE_A) != 0) {
      bitmask |= (1L << i);
      *pte &= flag;
    }
  }

  if(copyout(pgtbl, userbuf, (char*)&bitmask, sizeof(bitmask)) < 0) {
    return -1;
  }


  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
