#include "randomization.h"


Item *item_randomize_consumable(ItemPrototype *prototype)
{
    Item *item = item_clone(prototype);
    Consumable *consumable = &item->consumable;

    consumable->permanent = rand_true(0.1);
    consumable->type = (AttributeType) rand_in(0, PLAYER_ATTR_NUM);
    item->value = rand_in_range(prototype->value_range);

    if (consumable->permanent) {
        item->value /= 10;
    }

    return item;
}

Randomizable random_from(Probability *probability)
{
    uint16_t picked = rand_in(0, probability->sum);
    uint16_t cumulative = 0;

    for (int i = 0; i < probability->size; i++) {
        cumulative += probability->items[i].chance;

        if (picked <= cumulative) {
            return probability->items[i].value;
        }
    }
    fatal("Unable to pick random item");
}
