#ifndef ROGUECRAFT_ITEM_REGISTRY_H
#define ROGUECRAFT_ITEM_REGISTRY_H


#include "storage/cache.h"
#include "util/randomization.h"


typedef struct ItemPrototype ItemPrototype;


void item_registry_load(void);

void item_registry_add(const ItemPrototype *item, uint16_t chance);

CacheError item_cache_load(void);

void item_cache_save(const Probability *item_types);

const ItemPrototype *item_registry_random(void);

const ItemPrototype *item_registry_get(const char *name);

void item_registry_unload(void);


#endif
