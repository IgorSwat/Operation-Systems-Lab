#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int requests = 0;
int current_request = 0;
int is_request_awaiting = 0;
double time_meas = 0.0;

void print_numbers() {
    for (int i = 1; i < 101; i++)
        printf("%d ", i);
    printf("\n");
}

void print_curr_time() {
    time_t time_buff;
    struct tm* curr_time;
    time(&time_buff);
    curr_time = localtime(&time_buff);
    printf("Current time: %s\n", asctime(curr_time));
}

void print_requests_changes() {
    printf("Total request changes: %d\n", requests);
}

void print_time_repeat(double time_elapsed) {
    time_meas += time_elapsed;
    if (time_meas >= 1.0) {
        print_curr_time();
        time_meas = 0.0;
    }
}

void update_requests(int n_request) {
    if (n_request != current_request) {
        requests += 1;
        current_request = n_request;
    }
    is_request_awaiting = 1;
}

void handle_signal(int signo, siginfo_t* info, void* data) {
    int sender_id = info->si_pid;
    int request_code = info->si_status;
    if (request_code < 1 || request_code > 5) {
        printf("Incorrect signal catched\n");
        current_request = -1;
    }
    update_requests(request_code);
    kill(sender_id, SIGUSR2);
}

int main() {
    printf("Catcher id: %d\n", getpid());
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_handler = handle_signal;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);

    int finish_flag = 0;
    clock_t begin = clock();
    clock_t end = clock();

    while (finish_flag != 1) {
        double time_diff;
        if (begin < end)
            time_diff = (double)(end - begin) / CLOCKS_PER_SEC;
        else
            time_diff = 0.0;
        begin = clock();
        if (is_request_awaiting == 1)
        {
            switch (current_request)
            {
            case 1:
                print_numbers();
                break;
            case 2:
                print_curr_time();
                break;
            case 3:
                print_requests_changes();
                break;
            case 4:
                print_time_repeat(time_diff);
                break;
            case 5:
                printf("Catcher closed\n");
                finish_flag = 1;
                break;
            case 0:
                break;
            case -1:
                printf("Error: incorrect signal\n");
                exit(-1);
            }
            if (current_request != 4)
                is_request_awaiting = 0;
            end = clock();
        }
    }
    return 0;
}
