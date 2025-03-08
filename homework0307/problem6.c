#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void child_wait(pid_t pid);
void parent_wait(pid_t pid);

/* invoke wait() */
int main() {
    pid_t pid;
    pid = fork();
    child_wait(pid);
    // parent_wait(pid);
    return 0;
}

void child_wait(pid_t pid) {
    if (pid < 0) {
        fprintf(stderr, "Fork Failed!");
        exit(1);
    } else if (pid == 0) {
        pid_t wait_result = waitpid(getppid(), NULL, 0);
        printf("[Child] Wait Parent (pid:%d)'s Result is %d\n[Child] Own pid: %d\n", getppid(), wait_result, getpid());
    } else {
        printf("[Parent] Hello (pid %d), Child is %d\n", getpid(), pid);
    }
}

void parent_wait(pid_t pid) {
    if (pid < 0) {
        fprintf(stderr, "Fork Failed!");
        exit(1);
    } else if (pid == 0) {
        printf("[Child] Hello (pid %d), parent is %d\n", getpid(), getppid());
    } else {
        pid_t wait_result = waitpid(pid, NULL, 0);
        printf("[Parent] Wait Child (pid:%d)'s Result is %d\n[Parent] Own pid: %d\n", pid, wait_result, getpid());
    }
}
