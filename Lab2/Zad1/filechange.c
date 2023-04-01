#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

#define BUFF_SIZE 1024

#ifdef SYS_FLAG
    int transform_file(int argc, char* argv[]) {
    int in_file = open(argv[3], O_RDONLY);
    if (in_file == -1) {
        printf("Failed to open file %s \n", argv[3]);
        return -1;
    }
    int out_file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
    if (out_file == -1) {
        printf("Failed to open file %s \n", argv[4]);
        return -1;
    }
    char buffer[BUFF_SIZE];
    int length;
    while ((length = read(in_file, buffer, BUFF_SIZE)) > 0) {
        for (int i = 0; i < length; i++) {
            if (buffer[i] == *argv[1])
                buffer[i] = *argv[2];
        }
        write(out_file, buffer, length);
    }
    return 0;
}
#else
int transform_file(int argc, char* argv[]) {
    FILE* in_file = fopen(argv[3], "r");
    if (in_file == NULL) {
        printf("Failed to open file %s \n", argv[3]);
        return -1;
    }
    FILE* out_file = fopen(argv[4], "w+");
    if (out_file == NULL) {
        printf("Failed to open file %s \n", argv[4]);
        return -1;
    }
    char buffer[BUFF_SIZE];
    size_t length;
    while ((length = fread(buffer, sizeof(char), BUFF_SIZE, in_file)) > 0) {
        for (int i = 0; i < length; i++) {
            if (buffer[i] == *argv[1])
                buffer[i] = *argv[2];
        }
        fwrite(buffer, sizeof(char), length, out_file);
    }
    return 0;
}
#endif // SYS_FLAG

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Incorrect number of arguments\n");
        return -1;
    }
    if (strlen(argv[1]) != 1 ) {
        printf("Invalid argument: %s is not an ASCII character", argv[0]);
        return -2;
    }
    if (strlen(argv[2]) != 1 ) {
        printf("Invalid argument: %s is not an ASCII character", argv[1]);
        return -2;
    }
    struct timespec timespec_beg;
    struct timespec timespec_end;

    clock_gettime(CLOCK_REALTIME, &timespec_beg);
    int res = transform_file(argc, argv);
    clock_gettime(CLOCK_REALTIME, &timespec_end);

    printf("Execution real time: %ld ns\n", timespec_end.tv_nsec - timespec_beg.tv_nsec);
    return res;
}
