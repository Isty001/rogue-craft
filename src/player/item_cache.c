#include "../storage/cache.h"
#include "../../config/config.h"


#define cache_open_items(cache)                           \
    cache_open(cache, ITEM_CACHE, sizeof(CachedItem));    \


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
        default:
        fatal("No Probability found for ItemType [%d]", type)
    }
}

static void load_item(CachedItem *cached)
{
    ItemPrototype *prototype = allocate(sizeof(ItemPrototype));
    *prototype = cached->prototype;

    Probability *probability = probability_for(prototype->item.type);

    probability->items[probability->count].value = prototype;
    probability->items[probability->count].chance = cached->chance;
    probability->sum += cached->chance;
    probability->count++;
}

static ItemError load_from_cache(Cache *cache, time_t value_modified)
{
    if (cache_valid(cache, value_modified)) {
        cache_foreach(cache, (Reader) load_item);

        return IE_CACHE_LOADED;
    }
    cache_clear(cache);

    return IE_NO_CACHE_FOUND;
}

ItemError item_cache_load(void)
{
    if (!cache_exists(ITEM_CACHE)) {
        return IE_NO_CACHE_FOUND;
    }

    ItemError err;
    Cache cache;
    cache_open_items(&cache);

    time_t value_modified = dir_latest_modified_time(DIR_CONFIG_ITEMS);
    err = load_from_cache(&cache, value_modified);

    cache_close(&cache);

    return err;
}

static void save_items_in(Cache *cache, Probability *probability)
{
    CachedItem cached;

    for (int i = 0; i < probability->count; ++i) {
        cached.chance = probability->items[i].chance;
        memcpy(&cached.prototype, probability->items[i].value, sizeof(ItemPrototype));

        cache_add(cache, &cached);
    }
}

void item_cache_save(void)
{
    Cache cache;
    cache_open_items(&cache);

    for (int i = 0; i < ITEM_TYPE_PROBABILITY.count; i++) {
        save_items_in(&cache, ITEM_TYPE_PROBABILITY.items[i].value);
    }

    cache_close(&cache);
}
