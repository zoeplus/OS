#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int x = 100;
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork Failed!\n");
        exit(1);
    } else if (pid == 0) {
        printf("[Child] A child process (pid:%d)\n, parent is %d\n", getpid(), getppid());
        printf("[Child] Access x's value: %d\n", x);
        x = 10;
        printf("[Child] Change x's value to 10\n");
    } else {
        printf("[Parent] A parent process (pid:%d) of process %d\n", getpid(), pid);
        printf("[Parent] Access x's value: %d\n", x);
        x = 200;
        printf("[Parent] Change x's value to 200\n");
    }
    return 0;
}
