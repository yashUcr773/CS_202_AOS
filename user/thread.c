#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "kernel/riscv.h"
#include "user/user.h"
#include "user/thread.h"

int thread_create(void *(start_routine)(void*), void *arg) {
	// printf("[DEBUG] thread_create() - start...\n");
	void *stack;
	int pid;

	lock_t lk;
	lock_init(&lk);
	lock_acquire(&lk);
	// Set up new threads stack
	stack = malloc( PGSIZE * sizeof(void*));

	if (stack == 0) return -1;
	pid = clone((void*)stack);
	

	// pid == 0{}, run 4 time, but didn't call start_routine()
	// otherwise, it run 1 time, but call 
	if (pid == 0){
		// printf("[DEBUG] thread_create() - after clone, before start_routine...\n");
		start_routine(arg);
		// printf("[DEBUG] thread_create() - after start_routine...\n");
		free(stack);
		// printf("[DEBUG] thread_create() - after free...\n");
		exit(0);
	}
	lock_release(&lk);

	// printf("[DEBUG] thread_create() - ok...\n");
	return pid;
}



void lock_init(lock_t* lock) {
    lock->locked = 0;
}

void lock_acquire(lock_t* lock) {
	//same as spinlock.c line 32, 39
	while(__sync_lock_test_and_set(&lock->locked, 1) != 0);
	__sync_synchronize();
}

void lock_release(lock_t* lock) {
	//same as spinlock.c line 60, 69
	__sync_synchronize();
	__sync_lock_release(&lock->locked);
}
