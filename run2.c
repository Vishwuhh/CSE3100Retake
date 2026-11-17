
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t child;
    int firstStatus;
    int secondStatus;

    if (argc < 4) {
        fprintf(stderr,
                "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n",
                argv[0]);
        return 1;
    }

    child = fork();

    if (child == -1) {
        perror("fork()");
        return 1;
    }

    if (child == 0) {
        execlp(argv[1], argv[1], argv[2], (char *)NULL);
        perror("execlp()");
        _exit(127);
    }

    if (waitpid(child, &firstStatus, 0) == -1) {
        perror("waitpid()");
        return 1;
    }

    child = fork();

    if (child == -1) {
        perror("fork()");
        return 1;
    }

    if (child == 0) {
        execvp(argv[3], &argv[3]);
        perror("execvp()");
        _exit(127);
    }

    if (waitpid(child, &secondStatus, 0) == -1) {
        perror("waitpid()");
        return 1;
    }

    printf("exited=%d exitstatus=%d\n",
           WIFEXITED(firstStatus),
           WIFEXITED(firstStatus) ? WEXITSTATUS(firstStatus) : 0);

    printf("exited=%d exitstatus=%d\n",
           WIFEXITED(secondStatus),
           WIFEXITED(secondStatus) ? WEXITSTATUS(secondStatus) : 0);

    return 0;
}
