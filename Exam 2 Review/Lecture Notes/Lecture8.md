# Lecture 8 - Introduction to Processes
## Laundry Problem Conclusions
- multiple processes *at the same time* can lead to faster computation
  - this can be done on computers through the **fork() function**
## Fork Function Example
```C
#include <stdio.h>
#include <unistd.h>
int main() {
    pid_t value; // process identification to represent process id
    value = fork();
    printf("In main: %d\n", value); // prints id of proccess
}
```
- start in main, the program loads into memory
- create variable to identify process that we are working in
  - "laundry in child or parent "washing machine""
- calls the fork function and creates a *new* child process
  - the parent function is still running **at the same time**
- child process goes to the print statement and prints the id
  - children processes are given the id of **zero**
- parent process is given a unique value due tot he fork, and then prints out that id in the print statement 
> Why is the process id different in cloned versions of the code?
- because we want to write ONE code but have different parts of the code do different things *in parallel*
  - process ID helps us do that
```C
int main() {
    pid_t value;
    value = fork(); // seperates into parent and child 
    if(value != 0) { // what the parent process will do
        printf("Let's do the first load of laundry");
    } elseif(value == 0) { // what the child process will do
        printf("Let's do the second load of laundry");
    }
}
```
> Do the parent and child (clone) have independent memory?
- **variables are new copies in a new process**
## Process Basics
- instance of a program being executed
- in **multiprocessing** OS
  - multiple programs can be executed at once
  - multiple instances of a program can also be executed at once 
- executing miltiple programs 
  - single core: time sharing
  - multi core: true parallelism and time sharing 
## Process Management: OS View
- os maintains process table 
  - every process has an entry called the *process control block (PCB)*
- os **scheduler** picks processes to be executed at a given time 
  - when process is suspended, state is saved in PCB
## Process Management: User's View
- events that cause process creation
  - system initialization
  - user request for a new process (ex. **shell command**)
  - executing **shell script**, which can create many processes
- events which cause process termination
  - program exit (normal, error)
  - fatal error
  - killed by user command or signal
### Useful UNIX Commands for Processes
- ps
  - list running processes
- pstree
  - displays **tree** of processes
- top
  - dynamic view of memory and CPU usage
  - exiting top = press q
- kill
  - kill process given process ID
  - -9 option if simple kill does not work 
## Process Management: Programmer's View
- process birth
  - processes are always made by other processes
  - process always starts as a **clone** of parent process
    - process then could **upgrade itself** to run different executable 
    - child process retains access to files open in the parent
- process life
  - child process can create its own child processes
- process death
  - eventually calls **exit** or **abort** to commit suicide
  - can also get killed 
### Birth via Cloning
```C
#include <unistd.h>
pid_t fork(void);
```
- child is exact copy of the parent
  - both return from fork()
- **only difference is returned value**
  - in parent
    - fork() reutrns the process id of the child (>0)
    - if failure occurs, returns 1 and sets errno
  - in child 
    - fork() returns 0
## Concurrency
- parent and child processes return from fork() concurrently 
  - may return at the same time OR one after another 
    - order chosen by OS scheduler
## Cloning Effects
- on memory
  - parent and child memory are identical, but viewed as distinct by OS
  - memory changes affect only either child or parent 
  - parent and child can diverge quickly
- on files
  - **all files open in parent are accesible in child**
  - I/O operations in either one moves file position indicator
- in particular
  - *stdin, stdout, stderr* of parent are accessible in child 
> What can the parent do while child process is running?
- depends on application
  - can wait until child dies, run concurrently and check back on child later, or can run concurrently but **not check**
    - if child dies while the third case happens, it enters a **zombie** state
## Waiting on a Child 
```C
#include <sys/wait.h>

pid_t wait(int * status);
pid_t waitpid(pid_t pid, int * status, int options);
```
- purpose
  - blocks calling process until child is terminated
    - can also block until another status change
  - report status in *status
    - ignored if NULL is passed
    - if not NULL, shows
      - cause of death and exit status of child 
  - return value identifies the child process (or -1 for error)
  - **"man -S2 wait"** for full details 
## Zombies 
- dead process waiting to be checked by parent
  - cannot be killed because *it is already dead*
  - most resources are released, but still has an entry in process table
- parents need to check their kids
- when parent dies, "init" becomes new parent
  - then zombie children are reaped
## System Calls 
- API used to request services from OS kernel
  - fork()
  - *system class more expensive than normal function calls*
```C
man -S2 intro;
man -S2 syscalls;
```
## One More Forking Example
```C
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t value;
    value = fork();
    value = fork();
    printf("In main: value = %d\n", value);
    return 0; 
}
```
- parent process clones a child which starts running on line 8
  - line 8 in child is another call to fork
- then parent goes from line 7 to line 8, which is another call to fork, and that child starts at line 9
- IDs
  - parent = 371
  - child 1 = 372
  - child 2, 3 = 0
> Can we correctly predict the ORDER of the values that will be printed before the code runs?
- ***NO!***