#include <list.h>
#include "memory/memory.h"
#include "storage/storage.h"
#include "storage/cache.h"
#include "cell.h"
#include "config.h"


#define CELL_SIZE sizeof(CellPrototype)


#define cache_open_items(cache)                                  \
    cache_open(cache, CACHE_CONFIG_CELLS, CELL_SIZE);             \



static void load_cell(List *prototypes, CellPrototype *loaded)
{
    CellPrototype *cell = mem_alloc(CELL_SIZE);
    memcpy(cell, loaded, CELL_SIZE);
    prototypes->append(prototypes, cell);
}

CacheError cell_cache_load(List *prototypes)
{
    if (!cache_is_empty(CACHE_CONFIG_CELLS)) {
        return CE_NOT_FOUND;
    }

    Cache cache;
    cache_open_items(&cache);

    return cache_foreach_valid(&cache, env_json_resource_path(RESOURCE_CELLS), function(void, (void *loaded) {
        load_cell(prototypes, loaded);
    }));
}

void cell_cache_save(List *prototypes)
{
    Cache cache;
    cache_open_items(&cache);

    prototypes->foreach_l(prototypes, function(void, (void *prototype) {
        cache_add(&cache, prototype);
    }));

    cache_close(&cache);
}
