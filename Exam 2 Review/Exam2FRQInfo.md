## Rules to Memorize
```C
pid_t pid = fork();

if (pid < 0) {
    perror("fork()");
    exit(EXIT_FAILURE);
}
else if (pid == 0) {
    // CHILD PROCESS
    // Perform child operations

    _exit(0);
}
else {
    // PARENT PROCESS
    // Perform parent operations

    waitpid(pid, NULL, 0);
}
```
- fork() returns 0 in the child, the child PID in parent, and -1 on failure
  - child's ordinary memory changes **do not** change the parent's variable 
```C
int p[2];
pipe(p);

write(p[1], &value, sizeof(value));
read(p[0], &value, sizeof(value));
```
- p[0] is always the read end, and p[1] is always the write end 
  - pipe transfer bytes **not shared variables**
  - close unnecesary ends to prevant hangs plus allow EOF to be detected
```C
dup2(fd, STDIN_FILENO);   // fd becomes stdin
dup2(fd, STDOUT_FILENO);  // fd becomes stdout
dup2(fd, STDERR_FILENO);  // fd becomes stderr
```
- dup2(fd, 1) means that 1 referes to the same open file description as fd
  - can close original fd without losing redirected standard output
## FRQ Examples 
> Write a C program that executes two commands concurrently
```C
int main(int argc, char*argv[]) {
    int split = -1; // holds the posiiton of "--" in argv (-1 meaning seperator not found yet)

    // find where first command ends, and second command begins
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--") == 0) {
            split = i;
            break;
        }
    }
    // both commands needs to exist, with split <= 1 meaning command 1 missing, and split >= argc -1 means command 2 is missing 
    if (split <= 1 || split >= argc - 1) {
        fprintf(stderr,
                "Usage: %s cmd1 args -- cmd2 args\n",
                argv[0]);
        return 1;
    }
    // replaces '--' with NULL, since execvp requires a NULL-terminated argv array, and creates two usable arg arrays
    argv[split] = NULL;
    
    // create child 1
    pid_t pid1 = fork();

    if(pid1 < 0) {
        // fork returned -1, so the creation failed 
        perror('fork()');
        return 1;
    }
    if(pid == 0) { // child 1 ONLY
        // argv[1] is the executable name, and &argv[1] points to the beginning of command 1's arr
        execvp(argv[1], &argv[1]);
        // if it succeeds, code NEVER runs, reaching this means it failed
        perror('execvp()');
        // exit the failed child immediately
        _exit(127); // _exit() used since it is better for forking operations
    }
    // only original parent reaches this section, since the child either gets replaced or exited
    pid_t pid2 = fork(); // child 2
    
    if(pid2 < 0) {
        perror("fork()");

        // child 1 was already made, so reap it before parent terminates
        waitpid(pid1, NULL, 0);
        return 1;
    }
    if(pid2 == 0) { // child 2 ONLY
        // argv[split + 1] is second command, rest of argv is the argument array
        execvp(argv[split +1], &argv[split +1]);
        // runs only if execvp fails
        perror("execvip()");
        _exit(127); 
    }
    
    // original parent waits for children, and both children now exist can can run concurrently
    int status1, status2;
    // wait specifically for child 1
    if(waitpid(pid1, &status1, 0) == -1) {
        perror('waitpid()');
        return 1;
    }
    // wait specifically for child 2
    if(waitpid(pid2, &status2, 0) == -1) {
        perror('waitpid()');
        return 1;
    }
    
    // check how child 1 terminated
    
}
```