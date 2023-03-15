// thread.h 
typedef struct {
    uint locked;
} lock_t;


int thread_create(void *(start_routine)(void*), void *arg);
void lock_init(lock_t* lock);
void lock_acquire(lock_t* lock);
void lock_release(lock_t* lock);
