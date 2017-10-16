#ifndef ROGUE_CRAFT_CACHE_H
#define ROGUE_CRAFT_CACHE_H


#include <stdio.h>
#include <stdbool.h>
#include "util/randomization.h"
#include "storage.h"



#define RESOURCE_ITEMS "items"
#define RESOURCE_CELLS "cells"
#define RESOURCE_LEVELS "levels"
#define CACHE_COLORS "colors"



typedef void (*Reader)(void *entry);


typedef struct {
    char path[PATH_MAX];
    FILE *file;
    size_t entry_size;
} Cache;

typedef enum {
    CE_LOADED,
    CE_NOT_FOUND
} CacheError;


bool cache_exists(char *name);

bool cache_is_empty(char *name);

bool cache_valid(Cache *cache, time_t value_modified);

void cache_open(Cache *cache, char *name, size_t entry_size);

void cache_add(Cache *cache, void *entry);

void cache_foreach(Cache *cache, Reader read);

CacheError cache_foreach_valid(Cache *cache, char *last_modified_in, Reader reader);

void cache_clear(Cache *cache);

void cache_delete(char *name);

void cache_close(Cache *cache);


#endif
