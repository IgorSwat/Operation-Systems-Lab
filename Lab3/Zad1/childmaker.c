#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_INCONVERTBLE
} str2int_result;

str2int_result str2int(char* str, int* out) {
    char* end;
    if (str[0] == '\0' || isspace(str[0]))
        return STR2INT_INCONVERTBLE;
    long l = strtol(str, &end, 10);
    if (*end != '\0' && *end != '\n')
        return STR2INT_INCONVERTBLE;
    *out = l;
    return STR2INT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments - expected 1, got: %d \n", argc - 1);
        exit(1);
    }
    int no_childs = 0;
    if (str2int(argv[1], &no_childs) == STR2INT_INCONVERTBLE) {
        printf("Invalid argument type\n");
        exit(1);
    }
    int pid;
    for (int i = 0; i < no_childs; i++) {
        pid = fork();
        if (pid ==  0) {
            printf("ID procesu: %d, ID procesu macierzytego: %d \n", getpid(), getppid());
            exit(0);
        }
        else if (pid < 0) {
            perror("Failed to create new process\n");
            exit(-1);
        }
    }
    while (wait(NULL) > 0)
        continue;
    printf("%s \n", argv[1]);
    return 0;
}
