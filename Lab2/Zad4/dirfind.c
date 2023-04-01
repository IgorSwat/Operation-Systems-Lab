#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>

long long sumofs = 0;

int process_file(const char* fpath, const struct stat* st, int typeflag) {
    if (!S_ISDIR(st->st_mode)) {
        sumofs += st->st_size;
        printf("%ld %s \n ", st->st_size, fpath);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments\n");
        return -1;
    }
    if (ftw(argv[1], process_file, 20) != 0) {
        printf("Failed to execute function on directory: %s \n", argv[1]);
        return -1;
    }
    printf("Total size of files: %lld \n", sumofs);
    return 0;
}
