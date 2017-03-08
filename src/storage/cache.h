#ifndef ROGUE_CRAFT_CACHE_H
#define ROGUE_CRAFT_CACHE_H


#include <stdio.h>
#include <stdbool.h>
#include "../randomization.h"
#include "storage.h"


typedef void (*Reader)(void *entry);


typedef struct {
    char path[MAX_PATH];
    FILE *file;
    size_t entry_size;
} Cache;


void cache_init(char *dir);

bool cache_exists(char *name);

bool cache_valid(Cache *cache, time_t value_modified);

void cache_open(Cache *cache, char *name, size_t entry_size);

void cache_add(Cache *cache, void *entry);

void cache_foreach(Cache *cache, Reader read);

void cache_clear(Cache *cache);

void cache_close(Cache *cache);


#endif
