# Exam 2 Free Response Practice
> Consider the program below, assume fork() and waitpid() succeeds. Determine number of processes, the process treem printed values, and which outputs occurs before others
```C
int main(void) {
    setbuf(stdout, NULL);

    int x = 2;

    pid_t a = fork(); // creates child A with own copy of x

    if (a == 0) { // child A process
        x += 3; // increments to 5

        pid_t b = fork(); // child A creates child B with value x of 5

        if (b == 0) { // child B process
            x *= 2; // multiplies to 10
            printf("G: %d\n", x);
            _exit(0);
        }

        waitpid(b, NULL, 0); 
        printf("C: %d\n", x);
        _exit(0);
    }
    // parent process
    x = 7; // sets parent copy of x to 7
    printf("P: %d\n", x);

    waitpid(a, NULL, 0);
    printf("Done: %d\n", x); // prints Done: 7

    return 0;
}
```
- output is the same, but order is not completely fixed
> Parent creates a child, and the child terminates instantly, but the parent remains running and does not call wait() or waitpid(). Explain the process state, if the child is still executing, and what happens if the parent terminates before the child. 
- child becomes **zombie process** after terminating since parent did not collect termination status
  - wait(NULL) and waitpid(pid, NULL, 0) reaps the child by getting the status of its termination
- if the parent terminates before the child, the child becomes a **orphan**
  - adpoted by init or a different linux subreaper 
> Programmer wants to create exactly three chid processes and print each child's number, and the code is below. How many processes exists at the end of the program, how many prints are executed, and how should the code be corrected?
```C
for (int i = 0; i < 3; i++) {
    fork();
    printf("Child: %d\n", i);
}
```
- each process executed every later fork() unless it exits or skips it
  - before loop - 1 process
  - after i = 0 - 2
  - after i = 1 - 4
  - after 1 = 2 - 8
- since there are eight process, there are **seven new children**
  - print statement runs two times in 0, four in 1, and eight in 2 --> 14 times in total
- CORRECTED CODE
```C
pid_t pid[3];

for (int i = 0; i < 3; i++) {

    pid[i] = fork();

    if (pid[i] < 0) { // checks for an error in forking operation
        perror("fork()");
        exit(1);
    }

    if (pid[i] == 0) {

        // Only the newly created child prints.
        printf("Child: %d\n", i);

        // Prevent child from reaching next fork.
        exit(0);
    }
}

// Only original parent reaches this section.
for (int i = 0; i < 3; i++) {
    waitpid(pid[i], NULL, 0);
}
```
> A program executes the code below, summing the necessary headers and successful system calls. Determine the parent's printed value, contents of out.txt, and why the parent and child behave differently for variables versus file offsets
```C
int fd = open("out.txt",
    O_WRONLY | O_CREAT | O_TRUNC, 0644);

int x = 10;

write(fd, "AB", 2); // writes into out.txt "AB"

pid_t pid = fork(); // child A

if (pid == 0) { // child process
    x = 50; // modifies x ONLY for the child
    // parent has access to file too, so sees changes
    write(fd, "CD", 2); // adds "CD" into file
    close(fd);
    _exit(0);
}

waitpid(pid, NULL, 0); // waits for child to finish writing in file 

write(fd, "EF", 2); // file contains "ABCDEF" now
printf("Parent x = %d\n", x); // prints parent value of x, which is 10

close(fd);
```
> Identify three problems in the code below and rewrite correctly
```C
pid_t pid = fork();

// no check for fork() failure
if (pid == 0) {
    execlp("ls", "ls", "-l", NULL);
    printf("Finished executing ls\n");
    // if execlp() fails, child falls through and executes later code
}
// parent does not wait for the child, and program doesn't check if execlp() failed
printf("Parent continues\n");
```
- CORRECTED CODE
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    pid_t pid = fork();

    // Check for fork failure.
    if (pid < 0) {
        perror("fork()");
        return 1;
    }

    if (pid == 0) {

        // CHILD: Replace program with ls.
        execlp("ls", "ls", "-l", (char *)NULL);

        // Runs ONLY if execlp fails.
        perror("execlp()");

        // Prevent Child from running parent code if it failed
        _exit(127);
    }

    // PARENT: Wait for Child.
    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid()");
        return 1;
    }

    printf("Parent continues\n");
    return 0;
}
```
> Assume an executbale ./adder accepts any number of integer args and prints their sum. Write a launcher program that creates a child and executes ./adder using launcher's command-line arguments. The parent must wait for the child
> 
> ./launcher 10 20 30 should be the equivalent of ./adder 10 20 30
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    // Require at least one number as input
    if (argc < 2) {
        printf("Enter numbers\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        return 1;
    }

    if (pid == 0) {

        // child process

        // change argv[0] so the new program sees its own name as the first argument.
        argv[0] = "./adder";

        // execv takes the executable path and the entire argument array.
        execv("./adder", argv);

        // only reached if execv fails
        perror("execv()");
        _exit(127);
    }

    // parent process, waits for child
    waitpid(pid, NULL, 0);

    return 0;
}
```
- original argv[] contains
```
argv[0] = "./launcher"
argv[1] = "10"
argv[2] = "20"
argv[3] = "30"
argv[4] = NULL
```
- changing argv[0] to "./adder" makes entire arg array sutible for new executable
  - execv() is used since number of numeric arguments aren't fixed
> Asuume data.txt contains exactly "0123456789", predict the values printed by the code below
```C
int fd = open("data.txt", O_RDONLY); // opens file as read only

// does nothing!
char a[4] = {0};
char b[3] = {0};

read(fd, a, 3); // reads three bytes (012) and stores in a

lseek(fd, 2, SEEK_CUR); // skips two bytes (34)

read(fd, b, 2); // reads two bytes (56) and stores in b

printf("%s %s\n", a, b); // prints the valyes in a and b

close(fd); // closes file 
```
- cheatsheet 
    lseek(fd, 0, SEEK_SET)	Move to beginning
    lseek(fd, 5, SEEK_SET)	Move to byte offset 5
    lseek(fd, 2, SEEK_CUR)	Move forward two bytes
    lseek(fd, -1, SEEK_CUR)	Move backward one byte
    lseek(fd, 0, SEEK_END)	Move to end
> A file log.txt initially contains "HELLO", analyze the final contents after the code below completes
```C
// opens file, truncates contents (deletes), and sets to write only
int fd = open("log.txt", 
    O_WRONLY | O_TRUNC);

write(fd, "ABC", 3); // writes ABC
close(fd); // closes file

// sets to write only and appends onto the end of file
fd = open("log.txt",
    O_WRONLY | O_APPEND);

write(fd, "XYZ", 3); // appends XYZ into the string in text file
close(fd); // closes file
```
> Assume only fd 0, 1, and 2 are initially open, all system calls succeed. Look at the code below and determine the numerical values of fd, copy, and other, the final contents of both files, and which descriptors remain open immediately after the code. 
```C
```