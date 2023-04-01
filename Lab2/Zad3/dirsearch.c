#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];
    getcwd(buffer, BUFFER_SIZE);
    DIR* curr_dir = opendir(buffer);
    if (curr_dir == NULL) {
        printf("Failed to open the current directory\n");
        return -1;
    }
    long long sumofs = 0;
    struct dirent* file_entry = NULL;
    struct stat file_info;
    while ((file_entry = readdir(curr_dir)) != NULL) {
        stat(file_entry->d_name, &file_info);
        if (!S_ISDIR(file_info.st_mode)) {
            sumofs += file_info.st_size;
            printf("%ld %s \n", file_info.st_size, file_entry->d_name);
        }
    }
    if (errno != 0) {
        printf("Failed to read directory`s  entries\n");
        return -1;
    }
    printf("Total size of file entries: %lld \n", sumofs);
    return 0;
}
