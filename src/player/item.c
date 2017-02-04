#include <memory.h>
#include <mem_pool.h>
#include "../../config/config.h"


static MemPool *ITEM_POOL;


/**
* @see player.h
 */
static ItemError consume_non_permanent(Item *item, Attribute *attribute)
{
    int16_t new = attribute->current + item->value;
    uint16_t limit = attribute->limit;
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
    if (CONSUMABLE != parent->type) {
        return IE_INVALID_ARGUMENT;
    }

    Consumable *consumable = &parent->consumable;
    Attribute *attribute = player->attr.type_map[consumable->type];

    if (consumable->permanent) {
        attribute->max += parent->value;

        return IE_CONSUMED;
    }

    return consume_non_permanent(parent, attribute);
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
