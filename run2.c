#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    pid_t child;
    int exitStatus;

    // at least, there should be 3 arguments
    // 2 for the first command, and the rest for the second command
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }

    // TODO
        child = fork();

    if(child < 0) {
        perror("fork()");
        return 1;
    }

    if(child == 0) {
        execlp(argv[1], argv[1], argv[2], (char*)NULL);
        perror("excelp()");
        _exit(127);
    }

    if(waitpid(child, &exitStatus, 0)) {
        perror("waitpid()");
        return 1;
    }

    printf("exited=%d exitstatus=%s\n",
            WIFEXITED(exitStatus),
            WIFEXITED(exitStatus) ? WEXITSTATUS(exitStatus): 0);
    
    child = fork();
    
    if(child < 0) {
        perror("fork()");
        return 1;
    }

    if(child == 0) {
        execvp(argv[3], &argv[3]);
        perror("execvp()");
        _exit(127);
    }

    if(waitpid(child, &exitStatus, 0) == -1) {
        perror("waitpid()");
        return 1;
    }

    printf("exited=%d exitstatus=%d\n",
            WIFEXITED(exitStatus),
            WIFEXITED(exitStatus) ? WEXITSTATUS(exitStatus): 0);

    return 0;
}
