#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

// global system level variables
// initialize total system calls counter as 0
uint64 global_sys_calls_counter = 0;

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
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

  argint(0, &pid);
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

// body of defined syscall function.
// call a function in process to execute0.
// get first argument passed to the system call
// pass the argument to process level function
uint64
sys_sysinfo(void)
{
  int n;
  argint(0, &n);
  return get_sys_sysinfo(n, global_sys_calls_counter);
}

// body of defined syscall function.
// call a function in process to execute0.
// get first argument passed to the system call
// pass the argument to process level function
uint64
sys_procinfo(void)
{
  uint64 pinfo_pointer; // user pointer to struct pinfo
  argaddr(0, &pinfo_pointer);
  return get_sys_procinfo(pinfo_pointer);
}

// print the process statistics
uint64
sys_sched_statistics(void)
{
  return sched_statistics();
}

// set tickets to a process. The max set value can only be 10000 as per lab. 
// The number of tickets can not be negative.
uint64
sys_sched_tickets(void)
{
  int tickets;
  argint(0, &tickets);
  if (tickets > MAX_TICKETS)
  {
    tickets = MAX_TICKETS;
  }
  else if (tickets <= 0)
  {
    tickets = 0;
  }
  else
  {
    // do nothing
  }
  return sched_tickets(tickets);
}
// Implement the clone system call
uint64 
sys_clone(void)
{
  uint64 addr;
  argaddr(0, &addr);
  return clone((void *)addr);
}