#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

void read_dir(const char* path, const char* arg) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(1);
    }
    struct stat* file_stats = malloc(sizeof(struct stat));
    struct dirent* entry = malloc(sizeof(struct dirent));
    while ((entry = readdir(dir)) != NULL) {
        char path_extended[PATH_MAX];
        path_extended[0] = '\0';
        strcat(path_extended, path);
        strcat(path_extended, "/");
        strcat(path_extended, entry->d_name);
        if (stat(path_extended, file_stats) < 0) {
            perror("Unable to read file stats");
            exit(1);
        }
        if (S_ISDIR(file_stats->st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            int pid = fork();
            if (pid == 0)
                read_dir(path_extended, arg);
        }
        else {
            FILE* fin = fopen(path_extended, "r");
            int arg_size = strlen(arg);
            char file_data[arg_size];
            int data_size = fread(file_data, sizeof(char), arg_size, fin);
            if (data_size == -1)
                printf("Unable to read data from file %s \n", path_extended);
            else if (data_size == arg_size) {
                if (strcmp(file_data, arg) == 0)
                    printf("%s %d\n", path_extended, getpid());
            }
        }
    }
    free(file_stats);
    free(entry);
    while (wait(NULL) > 0)
        continue;
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Incorrect number of arguments: expected 2, got %d \n", argc - 1);
        exit(1);
    }
    read_dir(argv[1], argv[2]);
    return 0;
}
