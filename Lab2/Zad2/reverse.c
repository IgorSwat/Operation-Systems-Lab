#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#define BUFF_SIZE 1024

long get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    return fsize;
}

#ifdef SINGLE_BYTE
int reverse_file(char* in_file, char* out_file) {
    FILE* input = fopen(in_file, "r");
    if (input == NULL) {
        printf("Failed to open file %s\n", in_file);
        return -1;
    }
    FILE* output = fopen(out_file, "w+");
    if (output == NULL) {
        printf("Failed to open file %s\n", out_file);
        return -1;
    }
    char buffer[1];
    long in_length = get_file_size(input);
    fseek(output, in_length, SEEK_END);
    while (fread(buffer, sizeof(char), 1, input) > 0) {
        fseek(output, -1, SEEK_CUR);
        fwrite(buffer, sizeof(char), 1, output);
        fseek(output, -1, SEEK_CUR);
    }
    if (errno != 0) {
        printf("Failed to reverse file: %s\n", in_file);
        return -1;
    }
    return 0;
}
#else
int reverse_file(char* in_file, char* out_file) {
    FILE* input = fopen(in_file, "r");
    if (input == NULL) {
        printf("Failed to open file %s\n", in_file);
        return -1;
    }
    FILE* output = fopen(out_file, "w+");
    if (output == NULL) {
        printf("Failed to open file %s\n", out_file);
        return -1;
    }
    char buffer[BUFF_SIZE];
    int length;
    long in_length = get_file_size(input);
    fseek(output, in_length, SEEK_END);
    while ((length = fread(buffer, sizeof(char), BUFF_SIZE, input)) > 0) {
        char reversed[length];
        for (int i = length - 1; i >= 0; i--)
            reversed[length - 1 - i] = buffer[i];
        fseek(output, -length, SEEK_CUR);
        fwrite(reversed, sizeof(char), length, output);
        fseek(output, -length, SEEK_CUR);
    }
    if (errno != 0) {
        printf("Failed to reverse file: %s\n", in_file);
        return -1;
    }
    return 0;

}
#endif // SINGLE_BYTE



int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Incorrect number of arguments\n");
        return -1;
    }
    struct timespec timespec_beg;
    struct timespec timespec_end;

    clock_gettime(CLOCK_REALTIME, &timespec_beg);
    int res = reverse_file(argv[1], argv[2]);
    clock_gettime(CLOCK_REALTIME, &timespec_end);

    if (res == 0)
        printf("Execution real time: %ld ns\n", timespec_end.tv_nsec - timespec_beg.tv_nsec);
    return res;
}
