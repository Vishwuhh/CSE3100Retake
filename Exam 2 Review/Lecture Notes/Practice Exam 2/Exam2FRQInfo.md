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
     if (WIFEXITED(status1)) {
        // Child 1 ended normally.
        printf("Command 1 exit status: %d\n",
               WEXITSTATUS(status1));
    }
    else if (WIFSIGNALED(status1)) {
        // Child 1 was killed by a signal.
        printf("Command 1 signal: %d\n",
               WTERMSIG(status1));
    }

    // chck how child 2 terminated
    if (WIFEXITED(status2)) {
        printf("Command 2 exit status: %d\n",
               WEXITSTATUS(status2));
    }
    else if (WIFSIGNALED(status2)) {
        printf("Command 2 signal: %d\n",
               WTERMSIG(status2));
    }

    return 0;
}
```
> Create program with three processes (one parent, two children) that exchanges characters. Parent starts with 'a', and each reciever increments the character by 1 before passing forward. This exhange repeats for three rounds, with the final recieved character being the next round's starting character. 

> ORDER: P -> C1 -> C2 -> P -> C2 -> C1 -> P
- we need **six pipes**
  - p[0] - parent to c1
    - p[0][0] = child 1 reads from parent 
    - p[0][1] = parent writes to child 1
  - p[1] - c1 to c2
  - p[2] - c2 to parent
  - p[3] - parent to c2
  - p[4] - c2 to c1
  - p[5] - c1 to parent 
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// prints error and terminates current process
static void bad(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

// reads one character from a pipe
static char recieve(int fd)) {
    char c;
    // read() returns the number of bytes read, so if it returns 1, it is a success
    if(read(fd, &c, 1)!= 1) {
        bad("read()");
    }
}

// writes one character into a pipe 
static void send_char(int fd, char c) {
    // &c gives address of the character, and 1 is the number of bytes sent
    if(write(fd, &c, 1) != 1) {
        bad("write()");
    }
}

int main(int argc, char **argv) {
    // needs only one char as input
    // ./solution a
    if(argc != 2 || argv[1][0] == '\0' || argv[1][1] != '\0') {
        fprintf(
            stderr,
            "Usage: %s CHARACTER\n",
            argv[0]
        );
        return 1;
    }

    // disable stfout buffering so output can appear immediately
    setbuf(stdout, NULL);

    // create the six pipes
    // p[i][0] = read end; p[i][1] = write end
    int p[6][2] // goes from 0-5 and 0-1

    for(int i = 0; i < 6; ++i) {
        if(pipe(p[i]) == -1) {
            bad("pipe()");
        }
    }

    // create child 1, will have access to all pipes
    pid_t c1 = fork();
    if(c1 < 0) {
        bad("fork()");
    }
    if(c1 == 0) {
        // what child 1 keeps
        // p[0][0] - reads from parent
        // p[1][1] - writes to child 2
        // p[4][0] - reads from c2
        // p[5][1] - writes to parent

        // close unused pipe ends
        close(p[0][1]);
        close(p[1][0]);

        close(p[2][0]);
        close(p[2][1]);

        close(p[3][0]);
        close(p[3][1]);

        close(p[4][1]);
        close(p[5][0]);

        // repeat the communication three times, for the three rounds
        for(int i = 0; i < 3; ++i) {
            // exchange 1 - reads from parent
            char x = recieve(p[0][0]);
            printf("C1 read: %c\n", x);

            // exchange 2 - increments character, writes to c2
            ++x;
            send_char(p[1][1], x);

            // exchange 5 - reads from c2
            x = recieve(p[4][0]);

            // exchange 6 - increments character, writes to parent
            ++x;
            send_char(p[5][1], x);
        }
        // close used pipes when done
        close(p[0][0]);
        close(p[1][1]);
        close(p[4][0]);
        close(p[5][1]);

        // exit to stop child 1 from accessing parent code
        _exit(0);
    }

    // parent creates child 2
    pid_t c2 = fork();
    if(c2 < 0) {
        bad("fork()");
    }
    if(c2 == 0) {
        // what child 2 keeps
        // p[1][0] - read from c1
        // p[2][1] - write to parent
        // p[3][0] - read from parent
        // p[4][1] - write to c1

        // close unused pipe ends 
        close(p[0][0]);
        close(p[0][1]);

        close(p[1][1]);
        close(p[2][0]);

        close(p[3][1]);
        close(p[4][0]);

        close(p[5][0]);
        close(p[5][1]);

         // repeat the communication three times, for the three rounds
        for(int i = 0; i < 3; ++i) {
            // exchange 2 - reads from c1
            char x = recieve(p[1][0]);
            printf("C1 read: %c\n", x);

            // exchange 3 - increments character, writes to parent
            ++x;
            send_char(p[2][1], x);

            // exchange 4 - reads from parent
            x = recieve(p[3][0]);

            // exchange 5 - increments character, writes to c1
            ++x;
            send_char(p[4][1], x);
        }
        // close remaining pipes
        close(p[1][0]);
        close(p[2][1]);
        close(p[3][0]);
        close(p[4][1]);

        _exit(0);
    }

    // parent process keeps
    // p[0][1] - write to child 1
    // p[2][0] - read from child 2
    // p[3][1] - write to child 2
    // p[5][0] - read from child 1

    // close unused pipe ends
    close(p[0][0]);

    close(p[1][0]);
    close(p[1][1]);

    close(p[2][1]);
    close(p[3][0]);

    close(p[4][0]);
    close(p[4][1]);

    close(p[5][1]);

    // get starting character from argv
    char x = argv[1][0]; 
    // repeat communication three times 
    for(int i = 0; i < 3; ++i) {
            // exchange 1 - writes to c1
            send_char(p[0][1], x);

            // exchange 3 - reads from c2
            x = receive(p[2][0]);

            // exchange 4 - increments characterm write to c2
            ++x;
            send_char(p[3][1], x);

            // exchange 6 - reads from c1
            x = receive(p[5][0]);

            // print final char for the round
            printf("Round %d: parent received %c\n",
               i + 1, x);
        }
    // close reminaing pipes
    close(p[0][1]);
    close(p[2][0]);
    close(p[3][1]);
    close(p[5][0]);

    // wait for children to finish
    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);

    return 0;
}
```
> Implement a three stage command pipeline with redirection. The program mist have three children, two pipes, connect three executables with dup2(), redirect final output to file, and wait for all three children.

> cat input.txt | tr a-z A-Z | sort > output.txt
- descriptor operations needed
  - c1 (cat)
    - input fd 0 - default
    - output fd 1 - pipe 1 write end 
  - c2 (tr) 
    - input fd 0 - pipe 1 read end 
    - output fd 1 - pipe 2 write end 
  - c3 (sort)
    - input fd 0 - pipe 2 read end
    - output fd 1 - output file
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

static void die(const char *s) { // reports errors easier 
    perror(s);
    exit(1); 
}

int main(int argc, char **argv) {
    // argv[1] = input filename, argv[2] = output filename
    if (argc != 3) { // validates correct number of arguments 
        fprintf(stderr, 
                "Usage: %s IN OUT\n", argv[0]);
        return 1;
    }
    // a = car writes -> tr reads
    // b = tr writes -> sort reads
    int a[2], b[2]; // creates the two pipes

    if(pipe(a) == -1 || pipe(b) == -1) {
        die("pipe()");
    }

    // create c1
    pid_t c1 = fork();
    if(c1 < 0) {
        die("fork()");
    }
    if(c1 == 0) {
        // runs cat input.txt, and output needs to go into pipe A
        // redirect stdout (fd 1) to pipe A's write end, so anything written from cat goes into pipe A instead of in terminal 
        if(dup2(a[1], STDOUT_FILENO) == -1) {
            die("dup2()");
        }
        // close original pip ends, stdout already points to A
        close(a[0]);
        close(a[1]);

        close(b[0]);
        close(b[1]);
        // replace c1 with cat, which reads input.txt and writes content to stdout (pipe A)
        execlp("cat", "cat", argv[1], (char*)NULL);
        // runs only if execl FAILS
        perror("execlp()");
        _exit(127);
    }
    // create c2 (tr)
    pid_t c2 = fork();
    if(c2 < 0) {
        die("fork()");
    }
    if(c2 == 0) {
        // c2 runs tr a-z A-Z, and reads from pipe A and writes to pipe B
        // redirects stdin (fd 0) to pipe A's read end, and tr now recieves cat's output
        if(dup2(a[0], STDIN_FILENO) == -1){
            die("dup2()");
        }
        // redirects stdout (fd 1) to pipe B's write end, and tr sends its uppercase text into pipe B
        if(dup2(b[1], STDOUT_FILENO) == -1){
            die("dup2()");
        }
        // close original pipe ends, since stdin and stdout are connected to the correct pipes
        close(a[0]);
        close(a[1]);

        close(b[0]);
        close(b[1]);
        // replace c2 with tr, and convert lowercase letters into uppercase letters
        execlp("tr", "tr", "a-z", "A-Z", (char*)NULL);
        // runs only if execl FAILS
        perror("execlp()");
        _exit(127);
    }
    // create c3 (sort)
    if(c3 < 0){
        die("fork()");
    }
    if(c3 == 0){
        // runs sort, and reads from pipe B and writes into output.txt
        // redirect stdin (fd 0) to pipe B's read end, so sort now recieves tr's output
        if(dup2(b[0], STDIN_FILENO) == -1 ){
            die("dup2()")
        }
        // open output file
        // permissions make it WRITE ONLY, creates file if needed, erases previous contents
        // 0644 - file permissions
        int fd = open(argv[2],
                 O_WRONLY | O_CREAT | O_TRUNC,
                 0644);
        if(fd == -1){
            die("open()");
        }
        // redirects stdout (fd 1) into the output file, and anything sort prints will go into the output.txt file
        if(dup2(fd, STDOUT_FILENO) == -1 ){
            die("dup2()")
        }
        // original fd is no longer needed since stdout now points to the same open file
        close(fd);
        // close pipe ends
        close(a[0]);
        close(a[1]);

        close(b[0]);
        close(b[1]);
        // replace c3 with sort, so its stdin recieved pipe B and stdout writes to output.txt
        execlp("sort", "sort", (char *)NULL);

        perror("execlp()");
        _exit(127);
    }
    // original parent process
    // does not send or recieve data, since children handle everything
    close(a[0]);
    close(a[1]);

    close(b[0]);
    close(b[1]);
    /// wait for each child to finish, and they can do their commands concurrently since they have already been created
    if (waitpid(c1, NULL, 0) == -1)
        die("waitpid()");

    if (waitpid(c2, NULL, 0) == -1)
        die("waitpid()");

    if (waitpid(c3, NULL, 0) == -1)
        die("waitpid()");

    return 0;
 }
```
> Write a program which aloows a child to pick an integer from 0-1000, and sends the maxinimum allowed number to the parent. From this, the parent must guess a number and send it to the child 
> The child then replied with 1 if its too small, -1 if its too big, and 0 if its the correct answer. The parent then adjusts its search range, and once correct, the child send an ASCII message with the number of guesses that the parent then prints. 
- there needs to be **two pipes**
  - p2c - sending the parent guess to child
  - c2p - sending the feedback from child to parent 
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// sends one int through the pipe
void send_int(int fd, int value) { // first arg is which pipe descriptor being sent to, second is int being sent
    if(write(fd, &value, sizeof(value)) != sizeof(value)) {
        perror("write()");
        exit(1);
    }
}
// recieves one int through the pipe
int get_int(int fd) { // takes in pipe descriptor of pipe reading from
    int value;
    if(read(fd, &value, sizeof(value)) != sizeof(value)) { // reads a binary int and returns it 
        fprintf(stderr, "Read failed\n");
        exit(1);
    }
    return value;
}

int main(int argc, char *argv[]) {
    // check command-line argument, ex. ./guess 280
    if(argc != 2) {
        fprintf(stderr, "Usage: %s SECRET\n", argv[0]);
        return 1;
    }
    // convert argument into an integer
    char *end;
    long secret = strol(argv[1], &end, 10);

    // secret must be between 1 and 1000
    if(end == argv[1] || *end != '\0' || secret < 1 || secret > 1000) {
        fprintf(stderr, "Secret must be between 1 and 1000\n");
    }
    return 1; 

    // create two pipes, p2c (parent to child) and c2p (child to parent)
    int p2c[1], c2p[1];

    if(pipe(p2c) == -1 || pipe(c2p) == -1) {
        perror("pipe()");
        return 1; 
    }

    // create child 
    pid_t pid = fork();
    if(pid < 0) {
        perror("fork()");
        return 1;
    }
    if(pid == 0) {
        // child reads guesses from parents, and writes responses to them 
        close(p2c[1]);
        close(c2p[0]);

        // send max value to parent
        send_int(c2p[1], 1000);

        int_attempts = 0; // start counter for attempts 
        while(1) {
            // get parents guess
            int guess = get_int(p2c[0]);

            // determine if guess is too small, big, or correct
            int response;
            if(guess == correct) {
                respond = 0;
            } elif(guess < secret) {
                respond = 1; 
            } else {
                response = -1;
            }

            // send feedback to parent
            if(response == 0) {
                // response for a CORRECT answer
                char message[100];

                // create ASCII message
                int line = snprintf(message, sizeof(message), "Solved in %d attempts (secret %ld).\n", attempts, secret);
                // send message without NULL terminator
                if(len < 0 || len >= (int)sizeof(message) || write(c2p[1], message, (size_t)len) != len) {
                    perror("write()");
                    _exit(1);
                }

                break;
            }
        }
        // child finished communication
        close(p2c[0]);
        close(c2p[1]);

        _exit(0);
    }
    // parent process

    // parent writes guesses to child and reads feedback
    close(p2c[0]);
    close(c2p[1]);
    // recieve max value from child
    int low = 1;
    int high = get_int(c2p[0]);

    // binary search
    while(1) {
        // guess middle of current range
        int guess = low + (high - low) / 2;
        print("Guess: %d\n", guess);
        // send guess to child 
        send_int(p2c[1], guess);
        // recieve feedback from child
        int response  get_int(c2p[0]);
        // correct guess: stop guessing
        if(response == 0) {
            break;
        }
        // small: guess higher numbers
        if(response == 1) {
            low = guess + 1;
        }
        // big: guess lower numbers
        if(response == -1) {
            high = guess - 1;
        }
    }
    // read final message from child, that message ends with newline
    printf("Child says: ");

    char ch;

    do {
        // read one char at a time because message length is not fixed
        if(read(c2p[0], &ch, 1) != -1) {
            fprintf(stderr, "Read failed\n");
            return 1;
        }
        putchar(ch);
    } while(ch != '\n');
    // close pipes and wait for child
    close(p2c[1]);
    close(c2p[0]);

    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid()");
        return 1;
    }

    return 0;
}
```
> Write a C program that opens a file, text.txt, reads the file until it encounters the first comma, and stores everything BEFORE the comma in a buffer. It needs to use open() and read() instead of fopen() and fread(), do not include comma in result, store the result as a C string, prevent buffer overflow, and handle errors
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    // open file in read-only mode, and since open() returns a fd, if it return -1 the open() failed 
    int fd = open("test.txt", O_RDONLY);
    if(fd == -1) {
        perror("open()");
        return -1;
    }
    // create a buffer that can store 63 chars plus a newline char (64 total)
    char buffer[64];
    // stroes current character read
    char c;
    // tracks the number of chars stored
    int i = 0;
    // tracks when the text exceeded the available buffer capacity
    int truncated = 0;
    // read() returns either positive for bytes read, 0 for EOF, and -1 for error
    ssize_t n;

    // reads chars one at a time, with &c being address where byte is stored and 1 being the request for exactly one byte
    while((n = read(fd, &c, 1)) == 1) {
        // stop when first comma is found, break happens BEFORE storing it so the comma is excluded
        if(c == ',') {
            break;
        } 
        // stores char only if space remains for it and the newline char
        if(i < 63) {
            buffer[i] = c;
            i++
        } else { // cannot safely store additional bytes, but continue reading until comma or EOF
            truncated = 1;
        }
    }
    // handle read errors
    if(n == -1) {
        perror("read()");
        close(fd);
        return 1;
    }
    // null terminate string, since read() doesn't automatically add '\0'
    buffer[i] = '\0'; // without this, printing with %s could lead into unrelated memory

    // print collected text
    printf("Result: %s\n", buffer); // %s means we're printing buffer as a string
    // notify user if string could not fit into buffer
    if(truncated) {
        fprintf(stderr, "Warning: text was truncated\n");
    }

    // close file descriptor
    close(fd);
    
    return 0;
}
```
> Write a C program that when given a [3 x 3] matrix, it can calcualte the sum of its rows and prints them all. This program should have three child processes, one matrix row per child, and each child should calculate their sum and send it to the parent to print. 
- all three children write to the **same** pipe!
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// stores one child's results
// row is index of row processed, sum is the total of the processed row
typedef struct {
    int row;
    int sum;
} Result;

int main(void) {
    // create the [3 x 3] matrix
    int a[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // create one shared pipe for the children, each child writes to p[1] and parent reads from p[0]
    int p[2];

    if(pipe(p) == -1) {
        perror("pipe()");
        return 1;
    }
    
    // fork the three child processes, each iteration creates new child, and i determines which row is processed
    for(int i = 0; i < 3; i++) {
        pid[i] = fork()l
        if(pid[i] < 0) {
            perror("fork()");
            return 1;
        }
        if(pid[i] == 0) {
            // children only write to pipe, so remove read end
            close(p[0]);
            // creare result struct
            Result r;
            // assign row based on i
            r.row = i;
            // begin sum at zero
            r.sum = 0;

            // calculate row sum
            for(int j = 0; j < 3; j++) {
                r.sum += a[i][j]; // add each element in row i
            }

            // send struct to parent in one write() call
            // &r points to struct's memory and sizeof(r) sends all of its bytes
            // small write is atomic on a blocking POSIX pipe when successful, since it is below PIPE_BUF (4096)
            if(write(p[1], &r, sizeof(r)) != sizeof(r)) {
                perror("write()");
                _exit(1);
            }
            // child finished sending data
            close(p[1]);
            // we need to exit here, if not, child could continue the outer for loop and create unintended child processes
            _exit(0);
        }
    }
    // parent process, it only reads from the pipe
    close(p[1]);
    // array stores results in row order
    int sums[3] = {0};

    // read the three child results
    for(int i = 0; i < 3; i++) {
        Result r;
        // treat structure as raw bytes
        char *ptr = (char *)&r
        // read() can transfer fewer bytes tahn requesred, so repeat until full struct is read
        while(remaining > 0) {
            ssize_t n = read(p[0], ptr, remaining);

            if(n <= 0) {
                fprintf(stderr, "Pipe read failed\n");
                return 1;
            }

            // move buffer pointer ahead
            ptr += n;
            // reduce number of missing bytes
            remaining -= n;
        }
        // store sum using the row number
        // children might finish in any order, but r.row can tell is where the result belongs
        sums[r.row] = r.sum;
    }
    // no more results needed
    close(p[0]);

    // wait for the three children
    for (int i = 0; i < 3; i++) {
        waitpid(pid[i], NULL, 0);
    }

    // print results in row order
    for (int i = 0; i < 3; i++) {
        printf("Row %d sum: %d\n", i, sums[i]);
    }

    return 0;
}
```

```C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

double calculate(const char *filename) {
    FILE *f = fopen(filename, "r");

    if (f == NULL) {
        perror("fopen");
        exit(1);
    }

    double a, b, total = 0;
    char left, op, right;
    int terms = 0;

    while (1) {
        int n = fscanf(f, " %c %lf %c %lf %c",
                       &left, &a, &op, &b, &right);

        if (n == EOF)
            break;

        if (n != 5 || left != '(' || right != ')') {
            fprintf(stderr, "Invalid expression\n");
            fclose(f);
            exit(1);
        }

        double result;

        switch (op) {
            case '+':
                result = a + b;
                break;
            case '-':
                result = a - b;
                break;
            case '*':
                result = a * b;
                break;
            case '/':
                if (b == 0) {
                    fprintf(stderr, "Division by zero\n");
                    fclose(f);
                    exit(1);
                }
                result = a / b;
                break;
            default:
                fprintf(stderr, "Invalid operator\n");
                fclose(f);
                exit(1);
        }

        total += result;
        terms++;

        int c;

        do {
            c = fgetc(f);
        } while (c == ' ' || c == '\t' || c == '\r');

        if (c == EOF || c == '\n')
            break;

        if (c != '+') {
            fprintf(stderr, "Expected +\n");
            fclose(f);
            exit(1);
        }

        int next;

        do {
            next = fgetc(f);
        } while (next == ' ' || next == '\t');

        if (next != '(') {
            fprintf(stderr, "Incomplete expression\n");
            fclose(f);
            exit(1);
        }

        ungetc(next, f);
    }

    fclose(f);

    if (terms == 0) {
        fprintf(stderr, "Empty expression\n");
        exit(1);
    }

    return total;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file.txt\n", argv[0]);
        return 1;
    }

    int p[2];

    if (pipe(p) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        close(p[0]);
        close(p[1]);
        return 1;
    }

    if (pid == 0) {
        close(p[0]);

        double answer = calculate(argv[1]);

        if (write(p[1], &answer, sizeof(answer))
            != sizeof(answer)) {
            perror("write");
            close(p[1]);
            _exit(1);
        }

        close(p[1]);
        _exit(0);
    }

    close(p[1]);

    double child_result;

    if (read(p[0], &child_result, sizeof(child_result))
        != sizeof(child_result)) {
        fprintf(stderr, "Read failed\n");
        close(p[0]);
        waitpid(pid, NULL, 0);
        return 1;
    }

    close(p[0]);

    int status;

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        return 1;

    double parent_result = calculate(argv[1]);

    double difference = child_result - parent_result;

    if (difference < 0)
        difference = -difference;

    printf("Child result: %.2f\n", child_result);
    printf("Parent result: %.2f\n", parent_result);

    if (difference < 0.00000001)
        printf("CONFIRMED\n");
    else
        printf("MISMATCH\n");

    return 0;
}

```