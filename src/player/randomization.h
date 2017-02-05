#ifndef ROGUE_CRAFT_RANDOMIZE_H
#define ROGUE_CRAFT_RANDOMIZE_H


#include "item.h"


typedef const void *Randomizable;


typedef const struct {
    uint16_t count;
    uint16_t sum;
    struct {
        uint16_t chance;
        Randomizable value;
    } items[];
} Probability;


Item *item_randomize_value(ItemPrototype *prototype);

Randomizable random_from(Probability *probability);


#endif
