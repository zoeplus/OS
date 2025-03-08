#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Don't use wait() to block a process */
int main() {
    char child_msg[] = "hello\n";
    char parent_msg[] = "goodbye\n";
    int pipefd[2];
    int child_msg_flag = 0;
    pid_t pid;
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork Failed!\n");
        exit(1);
    } else if (pid == 0) {
        printf("[Child] %s", child_msg);
        close(pipefd[0]);
        child_msg_flag = 1;
        write(pipefd[1], &child_msg_flag, sizeof(int));
        printf("[Child] Message parent a var child_msg_flag\n");
        close(pipefd[1]);
    }
    else {
        close(pipefd[1]);
        read(pipefd[0], &child_msg_flag, sizeof(int));
        printf("[Parent] Get child_mes_flag: %d\n", child_msg_flag);
        if (child_msg_flag == 1)
            printf("[Parent] %s", parent_msg);
    }
    return 0;
}
