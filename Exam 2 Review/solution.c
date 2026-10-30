#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct { // struct for holding values IMPLEMENT.
    int a;
    int b;
    char op;
    int result;
} Operation;


int calculate(const char *filename) {
    FILE *f = fopen(filename, "r"); // opens the txt file

    // error opening file
    if (f == NULL) {
        perror("fopen");
        exit(1);
    }

    int  a, b = 0; // (a op b) 
    int result, total = 0; 
    char op, sep; // the operations and seperators
    int n;

    while ((n = fscanf(f, " ( %d %c %d )",
                       &a, &op, &b)) == 3) {

        if (op == '+') { // addition
            result = a + b;
        }
        else if (op == '-') { // subtraction
            result = a - b;
        }
        else if (op == '*') { // multiplication
            result = a * b;
        }
        else if (op == '/' && b != 0) {
            result = a / b;
            }
        else if(op == '%') {
            result = a % b;
        }
        else {
            fprintf(stderr, "Invalid operation\n");
            exit(1);
        }

        total += result;

        if (fscanf(f, " %c", &sep) != 1) {
            break;
        }
        if (sep != '+') {
            fprintf(stderr, "Invalid separator\n");
            exit(1);
        }
    }

    fclose(f);
    return total;
}


int main(int argc, char *argv[]) {
    // arg mismatch
    if (argc != 2) {
        printf("Usage: %s file.txt\n", argv[0]);
        return 1;
    }

    int p[2]; // creates the pipe for parent child 

    // error
    if (pipe(p) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork(); // child

    if (pid < 0) { // error
        perror("fork");
        return 1;
    }

    if (pid == 0) { // child process
        // only needs read
        close(p[0]);

        int answer = calculate(argv[1]);

        if (write(p[1], &answer, sizeof(answer)) != sizeof(answer)) {
            _exit(1);
        }

        close(p[1]);
        _exit(0);
    }
    close(p[1]); // close write end 

    // parent process
    int child_answer; // answer recieved

    if (read(p[0], &child_answer, sizeof(child_answer)) != sizeof(child_answer)) {
    
        close(p[0]);
        waitpid(pid, NULL, 0);
        return 1;
    }

    close(p[0]); // close read end 
    waitpid(pid, NULL, 0); // parent waits
    printf("Result: %d\n", child_answer);

    return 0;
}