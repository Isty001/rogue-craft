#include "storage.h"


void dir_check(char *dir)
{
    if (0 != mkdir(dir, 0700) && errno != EEXIST) {
        fatal("Unable to create directory [%s]", dir);
    }
}

FILE *file_open(char *path, char *mode)
{
    FILE *file = fopen(path, mode);

    if (NULL == file) {
        fatal("Unable to open [%s] with [%s] mode", path, mode);
    }

    return file;
}

bool file_exists(char *file)
{
    struct stat stats;

    return 0 == stat(file, &stats);
}

size_t file_size(FILE *file)
{
    fseek(file,0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}
