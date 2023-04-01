#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments - expected 1, got %d \n", argc - 1);
        exit(1);
    }
    printf("%s ", argv[0]);
    fflush(stdout);
    execl("/bin/ls", "ls", argv[1], NULL);
    perror("Failed to execute ls\n");
    return 1;
}
