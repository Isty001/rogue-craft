#include "randomization.h"


void item_randomize_consumable(Item *item)
{
    Consumable *consumable = &item->consumable;

    consumable->permanent = rand_true(0.1);
    consumable->type = (AttributeType) rand_in(0, PLAYER_ATTR_NUM);
    consumable->value = rand_in(10, 60);

    if (consumable->permanent) {
        consumable->value /= 10;
    }
}

Randomizable random_from(Probability *probability)
{
    uint16_t p = rand_in(0, probability->sum);
    uint16_t cumulative = 0;

    for (int i = 0; i < probability->size; i++) {
        cumulative += probability->items[i].value;

        if (p <= cumulative) {
            return probability->items[i].ptr;
        }
    }
    fatal("Unable to pick random item");
}
