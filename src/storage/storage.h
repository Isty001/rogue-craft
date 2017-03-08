#ifndef ROGUE_CRAFT_STORAGE_H
#define ROGUE_CRAFT_STORAGE_H


#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include "../util.h"


/** PATH_MAX is unnecessarily big for us */
#define MAX_PATH 300


void dir_check(char *dir);

FILE *file_open(char *path, char *mode);

bool file_exists(char *file);

size_t file_size(FILE *file);


#endif
