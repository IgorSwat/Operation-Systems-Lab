#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

void show_confirmation(int signo) {
    printf("Received confirmation\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Not enough erguments for sender\n");
        exit(-1);
    }
    int catcher_id = atoi(argv[1]);
    if (catcher_id <= 0) {
        printf("Error: invalid catcher id argument\n");
        exit(-1);
    }
    for (int i = 2; i < argc; i++) {
        int command = atoi(argv[i]);

        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_handler = show_confirmation;
        sigaction(SIGUSR2, &action, NULL);

        printf("Sending command: %d\n", command);
        sigval_t value;
        value.sival_int = command;
        sigqueue(catcher_id, SIGUSR1, value);

        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask, SIGUSR2);
        sigsuspend(&mask);
    }
    return 0;
}
