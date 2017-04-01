#include <memory.h>
#include <mem_pool.h>
#include "../../config/config.h"
#include "inventory.h"


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

Item *item_allocate(void)
{
    Item *item = pool_alloc(ITEM_POOL);
    profile_item(++);

    return item;
}

void item_clean(Item *item)
{
    if (item->clean) {
        item->clean(item);
    }
}

void item_free(Item *item)
{
    item_clean(item);
    pool_release(ITEM_POOL, item);
    profile_item(--);
}

Item *item_clone(ItemPrototype *prototype)
{
    Item *item = item_allocate();
    memcpy(item, &prototype->item, sizeof(Item));
    item->value = rand_in_range(prototype->value_range);

    return item;
}

Item *item_random(void)
{
    Probability *type = probability_pick(&ITEM_TYPE_PROBABILITY);
    ItemPrototype *prototype = probability_pick(type);

    return item_clone(prototype);
}

/**
* @see player.h
 */
static ItemError consume_non_permanent(Item *item, Attribute *attribute)
{
    int16_t new = attribute->current + item->value;
    uint16_t max = attribute->max;

    if (new > max) {
        attribute->current = max;
        item->value = new - max;

        return IE_REPEAT;
    } else if (new < 0) {
        attribute->current = 0;
        item->value = new;

        return IE_REPEAT;
    }
    attribute->current = new;

    return IE_CONSUMED;
}

ItemError item_consume(Item *parent, Player *player)
{
    check_type(parent, CONSUMABLE);

    Consumable *consumable = &parent->consumable;
    Attribute *attribute = &player->state.attributes[consumable->attribute];

    if (consumable->permanent) {
        attribute->max += parent->value;

        return IE_CONSUMED;
    }

    return consume_non_permanent(parent, attribute);
}

static void restore_occupied_cell(Item *item, Level *level, Point point)
{
    level->cells[point.y][point.x] = item->occupied_cell;
}

EventError item_pickup(InteractionEvent *event)
{
    Cell *cell = event->cell;
    Player *player = event->player;

    if (ITEM_ == cell->type && 1 == event->player_distance) {
        Item *item = event->cell->item;
        Level *level = player->level;

        if (IE_OK == inventory_add(player->inventory, item)) {
            if (item->occupied_cell) {
                restore_occupied_cell(item, level, event->point);
            } else {
                level_set_hollow(level, event->point);
            }
            item_clean(item);

            return EE_BREAK;
        }
    }

    return EE_CONTINUE;
}
