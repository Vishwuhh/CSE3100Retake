# Lecture 11: Pipes
> What happens if we want processes to exchange data?
- lets assume the child will do some computation, and we want to pass the value to the parent
```C
int main() {
    int solution[2]; // variable to fill in values
    pid_t pid = fork();
    if(pid == 0) { // child step
        // set value of function
        solution[0] = 10;
        solution[1] = 15;
        printf('In child=%d\n', solution[0])
        printf('In child=%d\n', solution[1])
    } else { // parent step
        sleep(2); // make parent wait for child
        printf('In parent=%d\n', solution[0])
        printf('In parent=%d\n', solution[1])
    }
    return 0; 
}
```
- pipes are techniques for inter-process communication
  - mechanism where output of one process is directed into the input of another process
- pipe file is created with pipe system call
  - has an input and output end 
  - can write into a pipe from an input end and read from the output end 
## Pipe Syntax
```C
#include <unistd.h>
int pipe(int pipefd[2]);
```
- creates one-way pipe
  - pipfd[0] - read end
  - pipefd[1] - write end 
  - returns 0 **if successful**
```C
int main() {
    int pd[2];
    pipe(pd);  // creates two new fd's for read and write end 
    int solution[2]; // variable to fill in values
    pid_t pid = fork();
    if(pid == 0) { // child step
        // set value of solution
        solution[0] = 10;
        solution[1] = 15;
        close(pd[0]); // closing pd[0] because child process is done
        write(pd[1], &solution, sizeof(solution)); 
        close(pd[1]);
    } else { // parent step
        printf("In the parent\n");
        close(pd[1]); // close pd[1] since we are at parent step
        read(pd[0], solution, sizeof(solution));
        printf('In parent=%d\n', solution[0]);
        printf('In parent=%d\n', solution[1]);
        close(pd[0]);
    }
    return 0; 
}
```
## More Notes on Pipes
-  what do you need to do if you need to make a two way communication between parent and child?
   -  two pipes
-  after exec, the new programn gets files descriptors of the pipe as well
   -  but an exe is only aware of fds 0, 1, 2 **but not 3 and 4**
## Building Pipe Setup
- we need to do the following