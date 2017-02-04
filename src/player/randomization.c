#include "randomization.h"


#define check_type(p, t) \
    if (t != p->item.type) return NULL;


Item *item_randomize_consumable(ItemPrototype *prototype)
{
    check_type(prototype, CONSUMABLE);

    Item *item = item_clone(prototype);
    Consumable *consumable = &item->consumable;

    consumable->permanent = rand_true(0.1);
    item->value = rand_in_range(prototype->value_range);

    if (consumable->permanent) {
        item->value /= 10;
    }

    return item;
}

Item *item_randomize_tool(ItemPrototype *prototype)
{
    check_type(prototype, TOOL);

    Item *item = item_clone(prototype);
    item->value = rand_in_range(prototype->value_range);

    return item;
}

Randomizable random_from(Probability *probability)
{
    uint16_t picked = rand_in(0, probability->sum);
    uint16_t cumulative = 0;

    for (int i = 0; i < probability->count; i++) {
        cumulative += probability->items[i].chance;

        if (picked <= cumulative) {
            return probability->items[i].value;
        }
    }
    fatal("Unable to pick random item");
}
