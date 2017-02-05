#include <memory.h>
#include <mem_pool.h>
#include "../../config/config.h"


#define check_type(i, t) \
    if (t != i->type) return IE_INVALID_ARGUMENT;


static MemPool *ITEM_POOL;


void item_pool_init(void)
{
    ITEM_POOL = pool_init(sizeof(Item), 100);
}

void item_pool_cleanup(void)
{
    pool_destroy(ITEM_POOL);
}

void item_free(Item *item)
{
    pool_free(ITEM_POOL, item);
}

Item *item_clone(ItemPrototype *prototype)
{
    Item *item = pool_alloc(ITEM_POOL);
    memcpy(item, &prototype->item, sizeof(Item));

    return item;
}

Item *item_random(void)
{
    Probability *type = random_from(&ITEM_TYPE_PROBABILITY);
    ItemPrototype *prototype = random_from(type);

    return prototype->randomize(prototype);
}

/**
* @see player.h
 */
static ItemError consume_non_permanent(Item *item, Attribute *attribute)
{
    int16_t new = attribute->current + item->value;
    uint16_t limit = attribute->modification_limit;
    bool exceeds_limit = attribute->increasing ? new < limit : new > limit;

    if (exceeds_limit) {
        attribute->current = limit;
        item->value = new - limit;

        return IE_REPEAT;
    }
    attribute->current = new;

    return IE_CONSUMED;
}

ItemError item_consume(Item *parent, Player *player)
{
    check_type(parent, CONSUMABLE);

    Consumable *consumable = &parent->consumable;
    Attribute *attribute = &player->attributes[consumable->type];

    if (consumable->permanent) {
        attribute->modification_limit += parent->value;
        attribute->increase_max += parent->value;

        return IE_CONSUMED;
    }

    return consume_non_permanent(parent, attribute);
}
