#include <string.h>
#include <regex.h>
#include <time.h>
#include <sys/times.h>

#ifdef DLL_FLAG
#include "butildl.h"    // Only for dll linking purposes
#else
#include "blockutil.h"
#endif // DLL_FLAG

#include "linkerdll.h"  // Only for dll linking purposes

#define BUFFER_SIZE 2048

int is_running = 1;     // Pseudo boolean variable (0 = false, 1 = true)
int is_table_initialised = 0;   // Pseudo boolean variable (0 = false, 1 = true)
regex_t init_reg;   // Regexes make easier the implementation of parsing user commands
regex_t count_reg;
regex_t show_reg;
regex_t delete_reg;
regex_t destroy_reg;
regex_t exit_reg;

typedef enum {
    INIT, COUNT, SHOW, DELETE, DESTROY, EXIT, ERROR     // Exit for user to end program, error if command is incorrect
} command_id;

void init_regs() {
    regcomp(&init_reg, "init [0-9][0-9]*", 0);
    regcomp(&count_reg, "count ..*", 0);
    regcomp(&show_reg, "show [0-9][0-9]*", 0);
    regcomp(&delete_reg, "delete [0-9][0-9]*", 0);
    regcomp(&destroy_reg, "destroy", 0);
    regcomp(&exit_reg, "exit", 0);
}

command_id get_command_type(char* input) {      // Parses string input to right command defined as part of enumerate
    if (regexec(&init_reg, input, 0, NULL, 0) == 0)
        return INIT;
    if (regexec(&count_reg, input, 0, NULL, 0) == 0)
        return COUNT;
    if (regexec(&show_reg, input, 0, NULL, 0) == 0)
        return SHOW;
    if (regexec(&delete_reg, input, 0, NULL, 0) == 0)
        return DELETE;
    if (regexec(&destroy_reg, input, 0, NULL, 0) == 0)
        return DESTROY;
    if (regexec(&exit_reg, input, 0, NULL, 0) == 0)
        return EXIT;
    return ERROR;
}

int get_input_len(char* input) {    // Gives the position of first encounter of \n in given string
    int i = 0;
    while (i < BUFFER_SIZE && input[i] != '\n')
        i += 1;
    return i;
}

command_id parse_input(char* input, int* numeric_arg, char* path_arg) {
    command_id input_id = get_command_type(input);
    switch (input_id) {     // Additional parsing for options arguments (such as init (num))
        case INIT:
            sscanf(input, "init %d", numeric_arg);
            break;
        case COUNT:
            input = input + 6;
            int nl_pos = get_input_len(input);
            strncpy(path_arg, input, nl_pos);
            break;
        case SHOW:
            sscanf(input, "show %d", numeric_arg);
            break;
        case DELETE:
            sscanf(input, "delete %d", numeric_arg);
            break;
        default:
            break;
    }
    return input_id;
}

void execute_cmd(mem_btable* table, command_id command, int* numeric_arg, char* path_arg) {
    int signal;
    if (is_table_initialised == 0 && command != INIT && command != EXIT) {  // If table is unitialised, only INIT and
        printf("Error: table uninitialised\n");                             // EXIT are safe and available for user
        return;
    }
    switch (command) {
        case INIT:
            if (numeric_arg < 0) {
                printf("Invalid argument value: %d \n", *numeric_arg);
                return;
            }
            create_table(table, *numeric_arg);
            is_table_initialised = 1;
            break;
        case COUNT:
            signal = read_block(table, path_arg);
            if (signal == 0) {
                printf("Failed executing wc on path %s \n", path_arg);
                return;
            }
            break;
        case SHOW:
            printf("%s", get_bdata(table, *numeric_arg));
            break;
        case DELETE:
            signal = remove_block(table, *numeric_arg);
            if (signal == 0) {
                printf("Failed to removed block with id: %d \n", *numeric_arg);
            }
            break;
        case DESTROY:
            free_btable(table);
            break;
        case EXIT:
            is_running = 0;
            break;
        case ERROR:
            printf("Error: incorrect command syntax\n");
            break;
    }
}

int main(int argc, char* argv[]) {
    if (load_dll() == 0)
        return -1;
    init_regs();
    mem_btable table;
    char buffer[BUFFER_SIZE];
    struct timespec timespec_beg;
    struct timespec timespec_end;
    struct tms tms_beg;
    struct tms tms_end;
    while (is_running != 0) {
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("Input closed\n");
            break;
        }
        int input_len = get_input_len(buffer) + 1;
        char input[input_len];
        strncpy(input, buffer, input_len);  // Used to cut the whole input line till the first encounter of \n
        int numeric_arg;
        char path_arg[BUFFER_SIZE];
        command_id command = parse_input(input, &numeric_arg, path_arg);

        clock_gettime(CLOCK_REALTIME, &timespec_beg);
        times(&tms_beg);
        execute_cmd(&table, command, &numeric_arg, path_arg);
        clock_gettime(CLOCK_REALTIME, &timespec_end);
        times(&tms_end);

        printf("Execution time:\n");
        printf("Real time: %ld ns\n", timespec_end.tv_nsec - timespec_beg.tv_nsec);
        printf("User time: %ld \n", tms_end.tms_cutime - tms_beg.tms_cutime);
        printf("System time: %ld \n\n", tms_end.tms_cstime - tms_beg.tms_cstime);
        // User and system time are always shown as 0 on my computer, but I am now sure how to fix it
    }
    return 0;
}
