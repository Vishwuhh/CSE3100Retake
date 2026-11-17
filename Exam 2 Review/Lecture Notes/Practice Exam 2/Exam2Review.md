# Exam 2 Review

## Multiple Choice
> In a scenario where a parent process has terminated but its child process is still running, what happens to the child process?
- The child **continues running as an oprhan process**
  - fork() creates a child process, which runs independently to the parent and the OS schedules independently 
    - if parent terminates **before** cild, child is not automatically terminated 
    - orphans are reparented to init and then reaped
> What is the logical error with the pseudo code provided?
```
1. Fork a child process.
2. If fork is successful, check if it's the child process.
3. If it's the child process:
    a. Execute a command using execvp, e.g., "ls -l".
    b. Print a message after the execvp call.
4. If it's the parent process:
    a. Wait for the child process to finish.
    b. Print a message indicating the child process has finished.
5. End of program
```
- the child **attempts to execute code after execvp()**
  - successful exec replaces the calling processes program, but **does not return to original code**
- consider
```C
pid_t pid = fork(); // creates child

if (pid == 0) { // child enters pid == 0 branch
    char *args[] = {"ls", "-l", NULL};

    execvp("ls", args); // loeads and executes ls -l
    // if successfuil child's program is replaced
    printf("Child finished\n"); // NEVER REACHED
}
else {
    wait(NULL); // parent waits for child to terminate
    printf("Parent finished\n"); // PARENT PRINTS THIS
}
```
> In the code below, how can the child process of the child process of the main parent process be identified?
```C
int main() {
    int a = fork();
    int b = fork();
    return 0; 
}
```
- a == 0 and b == 0
  - int a = fork()
    - orginal parent P had value a > 0
    - child C1 has a value a == 0
  - int b = fork();
    - P creates C2 and C1 creates C3 **at the same time**
      - P has a value of b > 0 and C2 has a value of b == 0
      - C1 has a value of b > 0
      - C3 has both a and b equal to zero
  - child of the first child is C3
  - total processes = 2 ^(number of fork calls)
    - does not apply when forks are inside conitional statements 
```C
if (fork() == 0) {
    fork();
}
```
- first child performs second fork, so there are three processes
  - Parent, Child 1, and Child 2
> What is the output for the code below ?
```C
int main() {
    execl("ls", "ls", "-1", NULL);
    printf("I live with the condifence that if I believe in myself, the money will follow.")
    return 0;
}
```
- execl() **does not** search PATH environment variable
  - because "ls" does not have a directory path, execl() attempts to execute ls in working directory
  - **this fails**
    - returns -1, execution continues to printf(), and directory listing is not displayed
```C
// Full executable path
execl("/bin/ls", "ls", "-l", (char *)NULL);
// Searches PATH
execlp("ls", "ls", "-l", (char *)NULL);
```
> Which of the following statements best describes the differences between fopen() and open() in C?
    > A. fopen() and open() are both part of the C standard library, but fopen() provides extra options for binary mode that open() does not support.
    > B. fopen() is declared in <stdio.h> and returns a pointer to a FILE structure that supports buffered I/O, whereas open() is declared in <fcntl.h> and returns an integer file descriptor used for unbuffered I/O.
    > C. fopen() can be used to open both files and directories, while open() can open only files.
    > D. Both fopen() and open() are system calls that directly interact with the kernel and provide identical functionality.
- **Option B**
  - both functions opens files, but provide different interfaces
    - fopen() declared in <stdio.h> while open() is declared in <fcntrl.h>
> Consider the text file test.txt containing "pizza", and the code below that reads the contents of test.txt
```C
int main() {
    int fd = open("test.txt", O_RDWR);
    char buff[10];
    int v = read(fd, buff, 10);
    return 0;
}
```
- **value v has 6 bytes**
  - ssize_t read(int fd, void *buffer, size_t count)
    - fd = file descriptor
    - buffer = memory where bytes are stored
    - count = max number of bytes requested
    - return value = num bytes read, 0 at EOF, -1 at error
  - "pizza" on test.txt is actually
  ```
  p i z z a \n
  ```
    - 6 bytes
  - even through the buffer can hold 10 bytes, siz is available
  - v = min(10, 6) so v has a value of 6
- NOTE
  - O_RDWR doesnt prevent read() from working
  - the following below is safer since read() doesnt automatically null-terminate strings
  ```C
  char buff[11];

  ssize_t n = read(fd, buff, 10);

  if (n >= 0) {
    buff[n] = '\0';
    printf("%s", buff);
   }
  ```
  > 