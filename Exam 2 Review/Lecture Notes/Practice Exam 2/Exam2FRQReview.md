# Soup du Jour

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Each pipe is named for the process that reads from it.
// Pipe 0: Parent/Child 2 -> Child 1
// Pipe 1: Parent/Child 1 -> Child 2
// Pipe 2: Child 1/Child 2 -> Parent
enum {
    TO_CHILD1, // assigned 0
    TO_CHILD2, // assigned 1
    TO_PARENT, // assigned 2
    PIPE_COUNT // assigned 3
};

// Helper function to send one character through a pipe.
void send_char(int fd, char value, const char *name)
{
    // write(fd, buffer, bytes) sends one byte.
    // We use &value because write() requires an address.
    if (write(fd, &value, 1) != 1) {
        perror("write");
        _exit(1);
    }

    // Print what this process sent.
    printf("%s wrote:%c\n", name, value);

    // Immediately display the output.
    fflush(stdout);
}

// Helper function to receive one character from a pipe.
char receive_char(int fd, const char *name)
{
    char value;

    // read() blocks until data becomes available
    // or the pipe reaches EOF (all writers close).
    if (read(fd, &value, 1) != 1) {
        fprintf(stderr, "%s: read failed or EOF\n", name);
        _exit(1);
    }

    printf("%s read:%c\n", name, value);
    fflush(stdout);

    // Return the received character.
    return value;
}

int main(int argc, char *argv[])
{
    // STEP 1: Check command-line arguments.
    // Example: ./solution a
    // argc = 2
    // argv[0] = "./solution"
    // argv[1] = "a"
    // argv[1][0] = 'a' -> the first char of the string

    if (argc != 2 || strlen(argv[1]) != 1) { // checks if user supplied one char arg
        fprintf(stderr,
                "Usage: %s <single-character>\n",
                argv[0]);
        return 1;
    }

    // STEP 2: Create three pipes.
    // p[i][0] = read end
    // p[i][1] = write end
    //
    // p[0] delivers messages to Child 1.
    // p[1] delivers messages to Child 2.
    // p[2] delivers messages to Parent.

    int p[PIPE_COUNT][2]; // two dim array of integers, with six descriptors

    for (int i = 0; i < PIPE_COUNT; i++) { // creates three pipes p[0], p[1], and p[2]
        if (pipe(p[i]) == -1) {
            perror("pipe");
            return 1;
        }
    } // before fork since children will inherit this

    // STEP 3: Create Child 1.

    pid_t child1 = fork(); // rerurns -1 if error, 0 if inisde child1, and childs PID if inside parent

    if (child1 == -1) {
        perror("fork child1");
        return 1;
    }

    if (child1 == 0) {

        // -------- CHILD 1 --------

        // Child 1 needs:
        // p[0][0] to RECEIVE from Parent/Child 2
        // p[1][1] to SEND to Child 2
        // p[2][1] to SEND to Parent
        //
        // Close the three ends it does not need.
        close(p[TO_CHILD1][1]);
        close(p[TO_CHILD2][0]);
        close(p[TO_PARENT][0]);

        // STEP 4: Receive from Parent.
        // Parent sends 'a'.
        // Child 1 reads 'a'.
        char c = receive_char(
            p[TO_CHILD1][0], // reading from pipe 0
            "Child 1"
        ); // reads from pipe 0

        // Increment the character.
        // 'a' + 1 = 'b'
        // Send 'b' to Child 2.

        send_char(
            p[TO_CHILD2][1], // where we are sending
            (char)(c + 1), // incrementing char
            "Child 1"
        );

        // STEP 5: Wait for Child 2 to send the next character back.
        // Child 2 eventually sends 'e'.
        // Child 1 reads 'e'.
        // read() blocks until 'e' is available.

        c = receive_char(
            p[TO_CHILD1][0], // reading again from pipe 0
            "Child 1"
        );

        // Increment 'e' to 'f'.
        // Send the final character to Parent.

        send_char(
            p[TO_PARENT][1], // sends char to parent
            (char)(c + 1), // increments char
            "Child 1"
        );

        // STEP 6: Child 1 is finished.
        // Close every remaining pipe descriptor.
        close(p[TO_CHILD1][0]);
        close(p[TO_CHILD2][1]);
        close(p[TO_PARENT][1]);

        // Terminate Child 1 so it does not execute the parent's remaining code.
        _exit(0); // does not do standard I/O cleanup, good for forked children
    }

    // STEP 7: Create Child 2.
    // Only the original parent reaches this fork,since Child 1 exits inside its own branch.

    pid_t child2 = fork(); // forking operation, parent forks since child terminates inside its own branch

    if (child2 == -1) { // checks for error forking
        perror("fork child2");
        return 1;
    }

    if (child2 == 0) {

        // -------- CHILD 2 --------

        // Child 2 needs:
        // p[1][0] to RECEIVE from Parent/Child 1
        // p[2][1] to SEND to Parent
        // p[0][1] to SEND to Child 1
        //
        // Close all unused ends.

        close(p[TO_CHILD1][0]);
        close(p[TO_CHILD2][1]);
        close(p[TO_PARENT][0]);

        // STEP 8: Receive from Child 1.
        // Child 1 sends 'b'.
        // Child 2 reads 'b'.

        char c = receive_char(
            p[TO_CHILD2][0], // where char is recieved from
            "Child 2"
        );

        // Increment 'b' to 'c'.
        // Send 'c' to Parent.

        send_char(
            p[TO_PARENT][1], // sends it to parent
            (char)(c + 1), // increments char
            "Child 2"
        );

        // STEP 9: Receive from Parent again.
        // Parent sends 'd'.
        // Child 2 reads 'd'.

        c = receive_char(
            p[TO_CHILD2][0], // recieves char from parent
            "Child 2"
        );

        // Increment 'd' to 'e'.
        // Send 'e' to Child 1.

        send_char(
            p[TO_CHILD1][1], // send char to child 1
            (char)(c + 1), // increments char
            "Child 2"
        );

        // STEP 10: Close all remaining
        // descriptors before terminating.

        close(p[TO_CHILD1][1]);
        close(p[TO_CHILD2][0]);
        close(p[TO_PARENT][1]);

        _exit(0);
    }

    // -------- PARENT PROCESS --------
    //
    // The original parent executes this section.
    //
    // Parent needs:
    // p[0][1] to SEND to Child 1
    // p[1][1] to SEND to Child 2
    // p[2][0] to RECEIVE from both children

    // STEP 11: Close unused pipe ends.

    close(p[TO_CHILD1][0]);
    close(p[TO_CHILD2][0]);
    close(p[TO_PARENT][1]);

    // STEP 12: Send the initial character
    // from the command-line argument.
    //
    // Example: argv[1][0] = 'a'
    // Parent sends 'a' to Child 1.

    send_char(
        p[TO_CHILD1][1],
        argv[1][0],
        "Parent"
    );

    // STEP 13: Receive from Child 2.
    //
    // Child 2 sends 'c'.
    // Parent receives 'c'.

    char c = receive_char(
        p[TO_PARENT][0], "Parent"
    );

    // Increment 'c' to 'd'.
    // Send 'd' back to Child 2.

    send_char(
        p[TO_CHILD2][1],
        (char)(c + 1),
        "Parent"
    );

    // STEP 14: Receive final character.
    //
    // Child 1 eventually sends 'f'.
    // Parent reads 'f'.
    //
    // This is the final communication.

    c = receive_char(
        p[TO_PARENT][0], "Parent"
    );

    // STEP 15: Close the parent's remaining
    // pipe descriptors.

    close(p[TO_CHILD1][1]);
    close(p[TO_CHILD2][1]);
    close(p[TO_PARENT][0]);

    // STEP 16: Wait for both child processes
    // to terminate and collect their exit statuses.
    //
    // Waiting happens AFTER communication to
    // prevent potential deadlocks.

    int status1, status2;

    if (waitpid(child1, &status1, 0) == -1) {
        perror("waitpid child1");
        return 1;
    }

    if (waitpid(child2, &status2, 0) == -1) {
        perror("waitpid child2");
        return 1;
    }

    // Ensure both children terminated normally
    // and returned successful exit codes.

    if (!WIFEXITED(status1) ||
        WEXITSTATUS(status1) != 0 ||
        !WIFEXITED(status2) ||
        WEXITSTATUS(status2) != 0) {
        return 1;
    }

    // All character exchanges are complete.
    return 0;
}
```