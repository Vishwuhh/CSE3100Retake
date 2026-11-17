# Resource Sharing for Threads
## Advantages of Threads
- simpler programming model
- easier to coordinate
- lighter weight than processes and takes less resources than processes to manage
- with substancial CPU and I/O, threads improve performance 
## Disadvantages of threads: What if one fails?
```C
// code to print time
void* basicAlarm(void *arg) {
    // choose how long till printing end of time
    int inc = *(int*)arg;
    // create time variable 
    int i;
    // iterate until time complete
    for(i = 0; i < inc; i++) {
        sleep(1);
        printf("Tick: sec Elapse=%d\n", i);
    }
    printf("Time is up!!!\n");
    pthread_exit(NULL);
}

// code with error in it
void codeWithError(void* inputInt) {
    // wait a little before doing the error
    sleep(5);
    // divide by zero
    int x = *(int*) inputInt;
    int y = 1;
    int z = x / (y-1);
}

int main(void) {
    pthread_t tid1, tid2;
    int timeToWait = 10;
    pthread_create(&tid1, NULL, basicAlarm, &timetoWait);
}
```
- one main disadvantage is the shared state of threads
  - if the CPU sees that a thread decides to sleep, it will go onto the next task that needs to be worked on 
## Disadvantages of Threads: Shared State
- threads share memoru and do computations on the same set of memory
  - sharing a single integer can go wrong
