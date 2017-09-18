#include <memory.h>
#include <mem_pool.h>
#include "sfx/sfx.h"
#include "inventory/inventory.h"


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

void item_free(Item *item)
{
    pool_release(ITEM_POOL, item);
    profile_item(--);
}

Item *item_clone(const ItemPrototype *prototype)
{
    Item *item = item_allocate();
    memcpy(item, &prototype->item, sizeof(Item));
    item->value = rand_in_range(prototype->value_range);
    item->name = prototype->name;

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
static ItemError consume_non_permanent(Item *item, State *state)
{
    int16_t new = state->current + item->value;
    uint16_t max = state->max;

    if (new > max) {
        state->current = max;
        item->value = new - max;

        return IE_CAN_BE_CONSUMED;
    } else if (new < 0) {
        state->current = 0;
        item->value = new;

        return IE_CAN_BE_CONSUMED;
    }
    state->current = new;

    return IE_CONSUMED;
}

ItemError item_consume(Item *parent, Player *player)
{
    check_type(parent, CONSUMABLE);

    Consumable *consumable = &parent->consumable;
    State *states = &player->state.map[consumable->state_type];

    if (consumable->permanent) {
        states->max += parent->value;

        return IE_CONSUMED;
    }

    return consume_non_permanent(parent, states);
}

static void restore_occupied_cell(Level *level, Item *item, Point point)
{
    Cell *current = level->cells[point.y][point.x];
    cell_free(current);

    level->cells[point.y][point.x] = item->occupied_cell;
    item->occupied_cell = NULL;
}

static void cleanup_light_source(Item *item, Player *player)
{
    Level *level = player->level;

    if (LIGHT_SOURCE == item->type) {
        level->lightings->delete(level->lightings, item->light_source.lighting);
    }
}

void item_pickup(LevelInteractionEvent *event)
{
    Cell *cell = event->cell;
    Player *player = event->player;

    if (ITEM_ == cell->type && 1 == event->player_distance) {
        Item *item = event->cell->item;
        Level *level = player->level;

        if (IE_OK == inventory_add(player->inventory, item)) {

            if (item->occupied_cell) {
                restore_occupied_cell(level, item, event->point);
            } else {
                level_set_hollow(level, event->point);
            }
            cleanup_light_source(item, player);
            sfx_play("player", "item_pickup");
        }
    }
}
