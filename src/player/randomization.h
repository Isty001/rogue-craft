#ifndef ROGUE_CRAFT_RANDOMIZE_H
#define ROGUE_CRAFT_RANDOMIZE_H


#include "item.h"


typedef const void *Randomizable;


typedef const struct {
    uint16_t size;
    uint16_t sum;
    struct {
        uint16_t value;
        Randomizable ptr;
    } items[];
} Probability;


void item_randomize_consumable(Item *item);

Randomizable random_from(Probability *probability);


#endif
