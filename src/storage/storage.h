#ifndef ROGUE_CRAFT_STORAGE_H
#define ROGUE_CRAFT_STORAGE_H


#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <tinydir.h>
#include <time.h>
#include "util/util.h"


typedef enum {
    CE_LOADED,
    CE_NOT_FOUND
} CacheError;


typedef void (*DirForeach)(tinydir_file *file);


void dir_check(char *dir);

void dir_foreach(char *dir, DirForeach foreach);

FILE *file_open(char *path, char *mode);

bool file_exists(char *file);

bool file_is_empty(char *path);

size_t file_size(FILE *file);

time_t dir_latest_modified_time(char *dir);


#endif
