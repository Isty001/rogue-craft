#include "config.h"
#include "item_registry.h"
#include "memory/memory.h"


#define cache_open_items(cache)                                  \
    cache_open(cache, CACHE_CONFIG_ITEMS, sizeof(CachedItem));    \


typedef struct {
    uint16_t chance;
    ItemPrototype prototype;
} CachedItem;


static void load_item(CachedItem *cached)
{
    ItemPrototype *prototype = mem_alloc(sizeof(ItemPrototype));
    *prototype = cached->prototype;

    item_registry_add(prototype, cached->chance);
}

CacheError item_cache_load(void)
{
    Cache cache;
    cache_open_items(&cache);

    return cache_foreach_valid(&cache, env_json_resource_path(RESOURCE_ITEMS), (Reader) load_item);
}

static void save_probability(Cache *cache, const Probability *probability)
{
    CachedItem entry;
    memset(&entry, 0, sizeof(CachedItem));

    probability_add_to_cache(cache, probability, entry, prototype, sizeof(ItemPrototype));
}

void item_cache_save(void)
{
    Cache cache;
    cache_open_items(&cache);

    for (int i = 0; i < ITEM_TYPE_PROBABILITY.count; i++) {
        save_probability(&cache, ITEM_TYPE_PROBABILITY.items[i].value);
    }

    cache_close(&cache);
}
