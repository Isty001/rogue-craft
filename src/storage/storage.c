#include <list.h>
#include "memory/memory.h"
#include "storage.h"


void dir_create(char *dir)
{
    if (0 != mkdir(dir, 0700) && errno != EEXIST) {
        fatal("Unable to create directory [%s]", dir);
    }
}

void dir_create_at(const char *base, const char *name, char *buff)
{
    if (!buff) {
        char path[PATH_MAX];
        buff = path;
    }
    sprintf(buff, "%s/%s", base, name);

    dir_create(buff);
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

bool dir_exists(char *file)
{
    struct stat stats;

    return 0 == stat(file, &stats) && S_ISDIR(stats.st_mode);
}

time_t dir_latest_modified_time(char *dir)
{
    time_t latest = 0;

    dir_foreach(dir, function(void, (tinydir_file * file)
        {
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

char *file_read(FILE *file)
{
    size_t size = file_size(file);
    char *buff = mem_alloc(size + 1);

    fread(buff, size, 1, file);
    buff[size] = '\0';

    return buff;
}

bool file_exists(char *file)
{
    struct stat stats;

    return 0 == stat(file, &stats);
}

bool file_is_empty(char *path)
{
    struct stat stats;

    return 0 == stat(path, &stats) && 0 != stats.st_size;
}

size_t file_size(FILE *file)
{
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}
