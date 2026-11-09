# Lecture 3: Locks
## Solution to Share Memory Problem: Pirates
- pirates faced similar problems to current programmers: they had treadure by did not want to share it 
  - they used a lock to help make sure things aren't taken by other pirates
## The Lock
- lock is a POSIX abstraction
  - POSIX = portable operating system interface
    - set of standard OS interfaces based on the UNIX OS
  - lock is a Mutex (Mutual Exclusion) and at most one thread can acquire the lock at any time 
## Race to Critical Section
- multiple threads race each other to the critical section
  - *critical section* = code segment that accesses shared resoruces
- one of the threads will "win" the race
  - winner
    - acquires a lock first
    - executed critical section
    - releases the lock
  - loser
    - waits as the resource is currently locked
    - wakes up when its their turn to get the lock
    - executes critical section
    - releases the lock
## Mutex (MUTual Exclusion)
```C
// define a mutex, a shared variable and only one thread can lock it
pthread_mutex_t mutex;
```
- at most one thread can acquire the lock at a time
  - if thread loses, it *falls asleep*
  - sleepy threads wake up when the lock is *released*
- mutex related abilities
  - initialize mutex
  - destroy mutex we no longer need
  - lock mutex
  - unlock a mutex
## Mutex Initalization
```C
#include <pthread.h>
int pthread_mutex_init(
    pthread_mutex_t* mutex, // pointer to a pthread_mutex_t
    const pthread_mutexattr_t* attr // attributes can be set for mutex, usually would be NULL
    );
```
- initializes a mutex (allocated OS resources)
- return 0 upon success
## Mutex Destruction
```C
#include <pthread.h>
int pthread_mutex_destroy(
    pthread_mutex_t* mutex // pointer to a pthread_mutex_t
    );
```
- releases resources for a lock
- returns 0 upon success
## Mutex Lock/Unlock
```C
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(
    pthread_mutex_t* mutex // pointer to a pthread_mutex_t
    );
```
- enters/leaves the critical section and waits if mutex is locked
- returns 0 if succesful, non zero means something went wrong
## Locking in Practice
- lock/unlock comes in pairs
  - need only *one mutex* per thing needing to protect, and it only protects that thing
- lock is BLOCKING
- critical sections should be sort because **other threads are waiting**
## Thread Locking Example with Structures
```C
// define structure with a lock
typedf struct SCounterTag {
    pthread_mutex_t _mtx;
    int _val;
} SCounter;

// method to make SCounter
SCounter * makeCounter() {
    SCounter * c = (SCounter*)malloc(sizeof(SCounter));
    if(c == NULL) {
        return c;
    }
    // initialize lock
    pthread_mutex_init*(&c->_mtc, NULL);
    // set counter value to zero
    c->_val = 0;
    return c;
}
//method to increment counter
void incCounter(SCounter* c) {
    pthread_mutex_lock(&c_->_mtx); // lock before increment
    c->_val++; 
    pthread_mutex_unlock(&c->_mtx); // unlock after
}
// method for incrementing using a thread
void *increaseBy100(void *p) {
    SCounter * x = (SCounter *p)p;
    for(int i = 0; i < 100; i++) {
        incCounter(x);
    }
    return NULL;
}
// method for freeing struct memory
// assume mutex is unlocked
void freeCounter(SCounter *c) {
    if(c == NULL) {
        return;
    }
    pthread_mutex_destroy(&c->_mtx);
    free(c);
}
// main function
int main(void) {
    // ids of threads
    pthread_t tid1, tid2;
    int status; // status for error handling

    SCounter* C = makeCounter();
    assert(x != NULL);

    // call increaseBy100 in two threads
    status = pthread_create(&tid1, NULL, increaseBy100, x);
    assert(status == 0);

    pthread_create(&tid2, NULL, increaseBy100, x);
    assert(status == 0);

    // we don't care about retval for now
    status = pthread_join(tid1, NULL);
    status = pthread_join(tid2, NULL);

    // could add a function to read the value too
    printf("counter is %d\n", x->_val);
    // free memory used for the struct
    freeCounter(x);
    return 0;
}
```
## Locking Granularity
- consider two options for an array where we want to do threading
  - option 1
    - wrap whole aray in structure
    - have a single lock for the whole thing 
  - option 2
    - wrap each value in a structure (value plus lock)
    - make an array of structures