#include <list.h>
#include "storage.h"


void dir_check(char *dir)
{
    if (0 != mkdir(dir, 0700) && errno != EEXIST) {
        fatal("Unable to create directory [%s]", dir);
    }
}

void dir_foreach(char *path, DirForeach foreach)
{
    tinydir_dir dir;
    tinydir_file file;

    if (0 != tinydir_open(&dir, path)) {
        fatal("Unable to open directory [%s]", path);
    }

    while (dir.has_next) {
        tinydir_readfile(&dir, &file);
        foreach(&file);
        tinydir_next(&dir);
    }
    tinydir_close(&dir);
}

time_t dir_latest_modified_time(char *dir)
{
    time_t latest = 0;

    dir_foreach(dir, function(void, (tinydir_file *file) {
        latest = max(latest, file->_s.st_mtim.tv_sec);
    }));

    return latest;
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
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}
