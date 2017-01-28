#include "item.h"


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
