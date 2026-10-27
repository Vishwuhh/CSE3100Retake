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
  - have parent process creating two child (child A and B)
  - each child runs executable
    - exectuable in A produces output
    - output of A is read as input into executable of child B
## High Level Strategy for Pipe Setup
- assume we want to launch two executables with two children
  - create pupe
  - fork 1
    - child process
      - redirect stdout to the write end of pipe
  - fork 2
    - child process
      - redirect stdin to the read end of pipe 
> How to set up in code?
- 'cat' executable gives text output
- 'tr' executable translates text
- code behavior
  - have parent create children A and B
    - child A creates text and writes to pipe with 'cat' executable
    - child B reads from pipe and uses 'tr' executable to convert text into capital letters
```C
int main(int arfc, char *argv[]) {
  int pd[2];
  if(pipe(pd) == -1) { // makes sure pipe is setup
    perror("Error");
    return -1
  }

  pid_t pid = fork(); // creates child A
  if(pid == 0) {
    dup2(pd[1], 1) // redirects stdout of child A to write end of pipe
    // closes file director 3 and 4
    close(pd[1]);
    close(pd[0]);
    // launches cat executable, takes pipe.c and turns into text
    char * argv_list = {"cat", "pipe2.c", NULL};
    execvp("cat", argv_list);
  }
  // child A we call exevp, so we are not returning anything
  // in parent, we close fd 4
  close(pd[1]);
  // now fd 3 of parent is in read end of pipe, and fd 1 of child A is in write end of pipe

  pid_t pid1 = fork(); // creating child B
  if(pid == 0) {
    dup2(pid[0], 0); // need to take input from pipe as fd 0 for child B, so use dup2
    // fd 0 of child B is connected to read end of pipe (for input)
    close(pd[0]); // close unneeded fd of pipe

    // calls new executable to translate lowercase to uppercase text
    char *argv_list[] = {"tr", "[a-z]", "[A-Z}]", NULL};
    execvp("tr", argv_list[]);
  }
  // close non-needed fds and wait for two processes to finish before returning
  close(pd[0]);

  waitpid(pid, NULL, 0);
  waitpid(pid1, NULL, 0);
  return 0;
}
```
## Going Further
- repeat for long pipelines
- draw pictures to find how pipes are used and what fds need to be closed
- REMEMBER
  - process are running in parallel once created
    - operations are in seuqence though
  - all processes in pipeline are run concurrently on Linux
    - when data is sent in pipe, next process picks up and starts to work
## Atomicity of read() and write()
```C
nr = read(fd, buf, N);
nw = write(fd, buf, N);
```
- returned values can be less than requested, and write()/read() returns number of bytes acturally read or written
- atomicity of write() is guaranteed if
  - num bytes is less than PIPE_BUF
  - bytes will be consecutive
  - default value of PIPE_BUF is 4096 on Linux
- read() is fine if al writes and reads are the same size
  - if not, needs special handling 
## Starting 2-stage Pipeline
```C
// A | B

pipe(pipefd) // pipefd is array of 2 ints
pid_a = fork(); // for child A
if(pid_a == 0) {
  dup2(); // setup stfout for A
  close both fds in pipefd
  exec to start A
}
close(pipefd[WR_END]); // dont need it open in parent

pid_b = fork(); // for child B
if(pid_b == 0) { // child process of B
  dup2(); // stdin for B
  close(pipefd[RD_END]);
  exec to start B // exit from child if on error
}
close(pipefd[RD_END]); // dont need it open in parent

// add code to check return val for errors
```
## Pipes for Summing Matrix Rows Concurrently 
```C
int main(void) {
  int i, row_sum, sum = 0, pd[2], a[N][N] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}};

  // creating pipe
  if(pipe(pd) == -1) {
    error_exit("pipe() failed");
  }

  for(i = 0; i < N; ++i) {
    if(fork() == 0) { // child process per row
      row_sum = add_vector(a[i]); // create child process for each row
      if(write(pd[1], &row_sum, sizeof(int)) == -1) { // writing to pipe
        error_exit("write() failed");
      }
      return 0;
    }
  }
  // better to close write end of parent
  for(i = 0; i < N; ++i) {
    if(read(pd[0], &row_sum, sizeof(int)) == -1) { // reading from pipe
        error_exit("read() failed");
      }
      sum += row_sum; // calculate total
  }

  printf("Sum of the array = %d\n", sum);

  // wait for child processes
}
```