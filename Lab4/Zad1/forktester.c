#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void handler(int signum) {
    printf("Handling signal SIGUSR1\n");
}

void check_for_signal() {
    sigset_t awaiting;
    sigpending(&awaiting);
    if (sigismember(&awaiting, SIGUSR1) == 1)
        printf("Signal SIGUSR1 awaiting  in process with id %d\n", getpid());
    else
        printf("Signal SIGUSR1 not awaiting  in process with id %d\n", getpid());
}

void block_signal() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0)
    {
        perror("Error during signal blocking\n");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Incorrect number of arguments - expected 1, got %d\n", argc - 1);
        exit(1);
    }
    char* command = argv[1];
    int pending_flag = 0;
    if (strcmp(command, "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(command, "handler") == 0) {
        signal(SIGUSR1, handler);
    }
    else if (strcmp(command, "mask") == 0) {
        block_signal();
    }
    else if (strcmp(command, "pending") == 0) {
        block_signal();
        pending_flag = 1;
    }
    else {
        printf("Incorrect command syntax\n");
        return 1;
    }
    raise(SIGUSR1);
    if (pending_flag > 0)
        check_for_signal();
    #ifdef EXEC
        if (argc != 3 || strcmp(argv[2], "executed") != 0) {
            execl(argv[0], argv[0], argv[1], "executed", NULL);
        }
    #else
    pid_t proces_id = fork();
    if (proces_id == 0) {
        if (pending_flag > 0)
            check_for_signal();
        else {
            raise(SIGUSR1);
            sleep(1);
        }
        exit(0);
    }
    else
        wait(NULL);
    #endif // EXEC
    return 0;
}
