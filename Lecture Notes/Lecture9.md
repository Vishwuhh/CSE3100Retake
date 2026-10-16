# Lecture 9: More on Processes
> What if we wanted the child to run some code that is NOT contained within the original file?
- look at seperate piece of simple code
- how can we run the code without forking?
- run the code WITH forking
## Look At Seperate Piece of Code
- adder.c 
```C
int main(int argc, char* argv[]) {
    int i, sum = 0;
    // converts string numbers to int data types
    // adds numbers
    for (i = 1; i < argc; i++) {
        sum += atoi(argv[i]);
    }
    printf("sum is: %d\n", sum); // prints total sum
    return 0; 
}
```
- argc = num arguments passed
- char* argv[] = numbers are input as strings from command line
## How Can We Run the Code Without Forking
```C
for (i = 1; i < argc; i++) {
        sum += atoi(argv[i]);
    }
    printf("sum is: %d\n", sum);
```
## Running Code With Forking
```C
int main() {
    char *cmd1 = "./adder.c";
    pid_t child = fork();
    if(child == 0) {
        printf("In child!\n");
        execl(cmd1, cmd1, "1", "2", "3", NULL);
        printf('Oops, something went really wrong!\n');
        perror(cmd1);
        return -1;
    } else {
        printf('In parent!\n');
        execl(cmd1, cmd1, "4", "6", NULL);
        printf('Oops, something went really wrong!\n');
        perror(cmd1);
        return -1;
    }
}
```
- child is cloned at line 2 and has 0 value
  - starts at line 8 and executes code
  - goes to line 9 and executes, but *does not return* 
  - should print 6
- parent starts at line 13 and executes code (has a **non-zero value**)
  - at line 15 it executes but *does not return*
    - should print 10
## Process Upgrades
- usually
  - fresh clone wants to run *different code* an is done by
    - loading another executable into the process address space
- NOTE  
  - **opened files are not affected by the upgrade operation**
## exec Family
- act of upgrading is done by child with system call
```C
#include <unistd.h>
int execl(const char *path, const char *arg0, ... /*, (char*) NULL */);
```
- path to executable to load inside our own address space
- list of args to be passed to the new executable
- *final* NULL pointer to give "end of arg list"
- if successful, execl() does not reutrn but started a new process
### How is Executable Found?
- specify a path like /bin/ls
- specify a file, and the system searches directories listed in PATH
  - echo $PATH in bash to see directories seperated by colons 
```C
int execl(const char *path, const char *arg0, .. /*, (char*)NULL*/);

// excelp searches paths for file 
int excelp(const char *file, const char *arg0, ... /*, (char*) NULL */);
```
## exec Family 2
```C
// if num arguments is unknown at compile time 
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[])
```
- args in execl are placed in an array
  - argv is the argv in main function
- execv needs a path, but execvp can *search file in PATH*
- starts a new process if successful like in execl
### Note on argv to execv and execvp
- last value in argv is null so that the code knows where to stop reading 
## File APIs
- IO APIs
  - "f" family
    - fopen, fclose, fread, fgetc, fscanf, fprintf
    - all use a FILE* abstraction to represent file 
  - UNIX has lower-level APIs for file handling
    - mapped to system calls (open, close, read)
    - use file descriptors (**which are just integers**)
    - deals with bytes only 
### How to Open a File
```C
#include <fcntl.h>
#include <unistd.h>

int open(const char *path, int oflag);
```
- parameters
  - path: path to the file that needs to be opened/created
  - oflag: read, write, read/write and more 
- function returns a *file descriptor*, small nonnegative integer
  - returns -1 on error
### Flags in open()
- must include one of the following
  - O_RDONLY, O_WRONLY, O_RDWR
- and or-ed with many optional flags too
  - O_TRUNC - truncates the file if opening a file for write
  - O_CREAT - creates a file if it does not exist
### Creating a File With open()
```C

```