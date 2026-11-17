## Lecture 4: POSIX Condition
## Producer-Consumer Problem
- thread 1 computes a result and thread 2 uses said result
  - produces put things into a shared buffer, and the consumer takes those out of the buffer
- *how does thread 2 know the result is ready?*
- constraints
  - mutual exclusion
    - one thread can manipulate the buffer at a time, since it is **shared**
  - synchronization
    - consumers must wait if buffer is **empty** and producers must wait if buffer is **full**
## Single Lock Solution
- consists of three parts
  - main
  - producer - function called by first thread
  - consumer - function called by second thread
```C
pthread_mutex_t queueLock;
int TaskAvailable;

main() {
    taskAvailable = 0;
    pthread_mutex_init(&queueLock, NULL);
    // other stuff
}
```
- main function
  - initialize one thread as producer, one as consumer (not shown)
  - create shared buffer as queue (not shown)
  - create lock
  - create taskAvailable global var for communication
```C
void *producer(void *threadData) {
    int inserted; // shows if new data is ready on the producer side (0 means able to put data, 1 is data is NOT ready)
    struct Task* myTask; // task the producer must do to make new data

    // repeated create new data and store data in shared bufferr IF conditions are right 
    while(!done()) {
        inserted = 0; // set to zero to get ready to add new data
        while(inserted == 0) { // tries to put new data in while possible
            LOCK(&queueLock); // checks condition 1 (access)
            if(taskAvailable == 0) { // checks condition 2 (room)

                // writes when conditions 1 and 2 are met
                insert_into_queue(myTask); // does task needed to make new data, is then consumed by consumer thread
                taskAvailable = 1; // tells consumer there is new data to process
                inserted = 1; // indicates no new data at this time
            }
            UNLOCK(&queueLock); // unlocks shared buffer and checks on the inserted while-loop
        }
    }
}
```
- *when can we give the consumer data?*
  - when we have access to our shared buffer to put data in
  - needs to have room in buffer to add new data 
    - consumer needs to consume everything
```C
void *consumer(void* threadData) {
    int extracted; // indicate if there is new data ready for consumer side (0 means put new data, 1 means not ready to put data)
    struct Task* myTask; // task structure filled in by the producer from the shared buffer

    // repeatedly checks buffer for new data and reads data if present
    while(!done()) {
        extracted = 0; // shows it is ready for new data
        while(extracted == 0) {
            LOCK(&queueLock); // locks buffer to read from it, if fails, thread goes to sleep
            if(taskAvailable == 1) { // checks for new data, var set to one in producer thread when it makes new data
                myTask = extract_from_queue*(); // gets task from buffer
                taskAvailable = 0; // set again to zero to show we are ready for new data
                extracted = 1; // to break out of inner while loop
            } 
            UNLOCK(&queueLock); // unlock because we are done with buffer
        }
        process(myTask); // consumes the data recieved
    }
}
```
## POSIX Condition Variable
- there is one issue with our solution
  - **we want to be able to write more into buffer, but are limited to one data piece**
- POSIX condition variable
  - second and last **core** synchronization primitive
    - every other can be implemented with mutex and condition variables 
  - always paired with a mutex
    - access needs to be *mutex protected*
  - predicate:
    - logic expression describing the property of an object (or state) the program needs
  - if the predicate is true, a thread can wait on a *condition variable*
    - other threads can signal on the condition variable when the predicate changed
      - this must be checked due to spurious wakeups
## pthread_cond API
```C
#include <pthread.h>

pthread_cond_t cond; // defines condition variable 

int pthread_cond_init(
    ptread_cond_t * cond,
    const pthread_condattr_t * attr
);

int pthread_cond_destroy(
    pthread_cond_t * cond
);

int pthread_cond_wait(
    pthread_cond_t * cond,
    pthread_mutex_t * mutex
);

int pthread_cond_signal(
    pthread_cond_t * cond
);

int pthread_cond_broadcast(
    pthread_cond_t * cond
);
```
## Typical Structre of Using mutex and cond
```C
// define mutex and cond
pthread_mutex_t mutex;
pthread_cond_t cond;

pthread_mutex_lock(&mutex); // gets mutex
while(! predicate) { // checks predicate
    pthread_cond_wait(&cond, &mutex); // waits on the condition and mutex is unlocked
    do_something(); // function done if needed, mutex is locked 
}
access_shared_resources() // can access shared resources safely
pthread_mutex_unlock(&mutex);

// perform other tasks like processing data
// access_shared_resources() should be short in length and runtime
```