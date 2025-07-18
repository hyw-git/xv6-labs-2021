#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"
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

// New add
uint64 sys_trace(void){
        int n;
        if(argint(0, &n) < 0){
                return -1;
        }
        struct proc *p = myproc();
        char *mask = p->mask;
        int i = 0;
        while(i < 23 && n > 0){
                if(n % 2){
                        mask[i++] = '1';
                }else{
                        mask[i++] = '0';
                }
                n >>= 1;
        }
        return 0;
}
// New add
uint64 sys_info(void){
        struct sysinfo info;
        uint64 addr;
        info.freemem = freebytes();
        info.nproc = proc_num();
        if(argaddr(0, &addr) < 0){
                return -1;
        }
        // copy info(kernel space) to addr(user space)
        if(copyout(myproc()->pagetable, addr, (char *)&info, sizeof(info)) < 0){
                return -1;
        }else{
                return 0;
        }
}       
