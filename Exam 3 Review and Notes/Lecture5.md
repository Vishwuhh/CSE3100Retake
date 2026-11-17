# Lecture 5: More on Conditionals, Locks, and Barriers
## Producer Consumer Code with Conditionals
```C
int maxBufferSize = 5; 
int bufferValue = 0; // would usually be a data struct like queue or array
pthread_cond_t readyCond; // set by producer, checked by consumer
pthread_cond_t processedCond; // set by consumer, checked by producer

// include locking variable
pthread_mutex_t mtx;

// full consumer section
void Consumer(void) {
    printf("Consumer thread.\n");
    int done = 0;
    while(1 != done) { // infitite loop for consumer checking buffer
        pthread_mutex_lock(&mtx); // acquires lock
        // consumer waits on ready conditions
        while(bufferValue == 0) { // break out and start to consume
            pthread_cond_wait(&readyCond, &mtx);
        }
        // taking from buffer
        bufferValue--;
        printf("Consumer just used buffer:%d\n", bufferValue);
        // unlock and go to sleep momentarily
        pthread_cond_signal(&processedCond);
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
}
// full producer section
void Producer1(void) {
    printf("Producer 1 thread.\n");
    int done = 0;
    while(1 != done) { // infinite loop where producer checks buffer to add
        pthread_mutex_lock(&mtx);
        // wait for process condition to be signaled
        while(bufferValue >= maxBufferSize) {
            pthread_cond_wait(&processCond, &mtx);
        }
        // write buffer value
        bufferValue++;
        pritnf("Prodiucer 1 just added to the buffer:%d\n", bufferValue);
        // signal ready condition
        pthread_cond_signal(&readyCond);
        pthread_mutex_unlock(&mtx);
        sleep(1)
    }
}
// main function
int main(void) {
    // initialize mutex and conditional variable objs
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&readyCond, NULL);
    pthread_cond_init(&processedCond, NULL);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, (void*)Consumer, NULL);
    pthread_create(&tid2, NULL, (void*)Producer1, NULL);
    pthread_create(&tid2, NULL, (void*)Producer2, NULL);
    // wait for threads to launch and signal consumer is ready
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    // destroy lock at end 
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&readyCond);
    pthread_cond_destroy(&processedCond);
    return 0;
}
```
## The Big Picture
- producer adds to buffer when
  - buffer not full
  - producer gets signal when checking readyCond
  - producer then signals using processedCond
- consumer takes from buffer when 
  - buffer has something in it
  - consumer gets signal through processedCond
  - consumer signals using readyCond
## More Synchronization Primitives
- read-write blocks
- barriers
- both can be built through mutexes and conditional variables 
## Readers Writers Problem
- many apps have a data structure being **read** frequently but **written** infrequently 
- constraints
  - multiple readers can read simultaneously 
  - reader can start even when others are reading
  - only one writer can write at a time
  - reading and writing cannot happen at the same time 
  - reader has to wait if a writer is writing, and a writer has to wait if there are readers present
## Read-Write Lock Example
```C
// read and write lock
pthread_rwlock_t rwlock; // supprts multiple readers and one writer
int bufferValue = 0; // global shared valuie

void ReaderA(void) {
    printf("Reader A Thread\n");
    int done = 0;
    // lock thread for reading
    while(done != 1) {
        
    }
}
```