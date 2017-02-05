#include "randomization.h"


#define check_type(p, t) \
    if (t != p->item.type) return NULL;


Item *item_randomize_value(ItemPrototype *prototype)
{
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
