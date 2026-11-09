# Lecture 1: An Introduction to Threads
- threading vs. fork
  - forks need two pipes to communicate between two processes
  - threads communicate with no pipes because they are within the same "box"
## Basic Idea of Threads
- tasks need to run in paralllel **and** SHARE the same memory spaxce
  - do not need to make pipes in this case
  - each tasks you want to run is done through a new thread
- *what are the difficulties?*
  - threads need to be told when to terminate and the main process needs to wait for the threads to finish
## Basic Parts of Parallel Sorting Code
```C
void sortIntsWithThread(void* pointerToArray) { // needs to point to array we are sorting 
    int* a = (int*)pointerToArray; // typecase pointer to be an integer pointer to actually use it
    int numElements = 5; // hardcoded number elements to sort
    // sorting based on method inputs
    qsort(a, numElements, sizeof(int), compare_int);
    // make sure thread exits
    pthread_exit(NULL);
}


int main() {
    // create test case
    int seed = 0;
    srand(seed); // sets seed to value

    // creates array u with random ints
    // have pointer to the start fo the array a, and middle of the array b
    int n = 10;
    int u[n];
    int *a, *b;
    int half = n/2;
    a = u;
    b = a + half;
    for(int i = 0; i < n; i++) {
        u[i] = rand() % n;
    }
    // check array before sorting
    for(int i = 0; i < n; i++) {
        printf("Array u[%d] = %d\n", i, u[i]);
    }
    
    // create two thread IDs to keep track of threads
    pthread_t = idA;
    pthread_t = idB;
    // sort first half of list with threadA
    pthread_create(&idA, NULL, (void*)sortIntsWithThread, a);
    // sort the first hald of list with threadB
    pthread_create(&idB, NULL, (void*)sortIntsWithThread, b);

    // wait for threads to finish
    pthread_join(idA, NULL);
    pthread_join(idB, NULL);
    merge(a, b, sorted, half);
    pritnf("After sorting\n");
    for(int i = 0; i < n; i++) {
        printf("Array u[%d] = %d\n", i, sorted[i]);
    }
    return 0;
}
```
## Process and Thread Definitions
 - process = bundle grouping
   - virtual address space [memory]
   - collection of files [IO]
   - collection of *concurrent threads* [execution units]
 - threads are lightweight entities
   - runs concurrently with other threads
   - share resources within the process [has same rights]
   - confined to single process [can't move to other processes]
   - can be created and destroyed [diff life cycles]
 - ***lightweight processes that chare address space in a process***
## Thread Creation and Destruction
- threads created with other threads
  - pthread_create()
  - given a stack to execute and function to run
- what about first thread?
  - process being created leads to a single thread
- how do threads die?
  - voluntarily after completing task
  - pthread_exit()
    - requested by *another* thread
  - pthread_cancel()
    - when process dies with all associated threads
      - any thread can call exit(), and is called when main() returns 
  ## Thread Syntax: Creation
  - need threading library #include <pthread.h>
  - then call *pthread_create*: pthread_t id;
```C
int pthread_create(pthread_t* thread, pthread_attr_t*, attr, void*(*start_routine)(void*), void* arg); 
// returns 0 if successful and non zero (positive) if error reached
```
- pthread_t* thread = id of thread being created
- pthread_attr_t* attr = attr specifies attribute for the thread
  - NULL is the default, but can modify the size of stack mem the thread is given or how the thread should be scheduled 
-  start_routine()= function the thread will run
   -  function can ony take one argument which is a void type pointer
-  void *arg = passed to start_routine() 
   -  argument to the function that the thread is going to run
## Thread Creation in Practice
- threads are peers once created, can can create their own threads
  - no implied heiarchy or dependency, **all are equal**
## Thread Concurrency
- single core cpu: time sharing
  - when one thead is waiting for IO to complete, another can use the CPU for computation
  - any one point in time, CPU is used by ***one thread***
- multi-core cpu: true concurrency
  - threadfs execute in parallel, with one per core
## Thread Syntax: Thread Termination
```C
void pthread_exit(void *status)
```
- call pthread_exit()
  - function always succeeds and doesn't return value
  - status obtained by other threads
- similar to process termination
  - main() returns, or exit() called by any thread
## Thread Syntax: Waiting for Threads
```C
int pthread_join(pthread_t tid, void** status)
```
- equivalent to waitpid() in processes
  - waits for thread to complete
  - blocks calling thread until thread tid terminates
  - can obtain exit status from thread, and passes NULL to ignore return value
## How to Return Data from Threads
> What happens if we want the method called by thread to return values?
>
> What happens if the method needs to take in multiple values?
- **POINT TO A STRUCT**
### Example
```C
 #define NUM_THREADS 8
 struct thread_data {
    int thread_num; // tracks thread number
    char* message; // message saved as string
    int len; // result
 }

 void* PrintHello(void* threadarg) {
    // convert void pointer to struct pointer
    struct thread_data* my_data = (struct thread_data*) threadarg;
    // wait for some time based on thread number
    sleep(1+ 5*(my_data->thread_num %2));
    // set length var to the length of the message 
    my_data -> len = strlen(my_data->message);
    // print thread number, message, and length of message
    printf("Thread #%d: %s length=%d\n", my_data -> thread_num, my_data->message, my_data->len);
    // terminate thread
    pthread_exit(NULL);
 }
```
- OS is in charge of executing threads, meaning if a thread computation time is high, OS could start working on another
## Threading Summary
- when you have takss that manipulate same memor and don't want independent copies, **use threading**
  - threads SHARE memory
- functions need to take in void* pointer and return a void* or just void to be called by threads
- **cannot predict which threads finish first**