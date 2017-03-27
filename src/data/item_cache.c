#include "../storage/cache.h"
#include "../../config/config.h"


#define cache_open_items(cache)                                  \
    cache_open(cache, CACHE_CONFIG_ITEM, sizeof(CachedItem));    \


typedef struct {
    uint16_t chance;
    ItemPrototype prototype;
} CachedItem;


static Probability *probability_for(ItemType type)
{
    switch (type) {
        case TOOL:
            return &ITEM_TOOL_PROBABILITY;
        case CONSUMABLE:
            return &ITEM_CONSUMABLE_PROBABILITY;
        case LIGHT_SOURCE:
            return &ITEM_LIGHT_SOURCE_PROBABILITY;
        default:
            fatal("No Probability found for ItemType [%d]", type)
    }
}

static void load_item(CachedItem *cached)
{
    ItemPrototype *prototype = allocate(sizeof(ItemPrototype));
    *prototype = cached->prototype;

    Probability *probability = probability_for(prototype->item.type);
    probability_add(probability, cached->chance, prototype);
}

CacheError item_cache_load(void)
{
    if (!cache_is_empty(CACHE_CONFIG_ITEM)) {
        return CE_NOT_FOUND;
    }

    Cache cache;
    cache_open_items(&cache);

    return cache_foreach_valid(&cache, DIR_CONFIG_ITEMS, (Reader) load_item);
}

static void save_probability(Cache *cache, Probability *probability)
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
