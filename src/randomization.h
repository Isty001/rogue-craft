#ifndef ROGUE_CRAFT_RANDOMIZE_H
#define ROGUE_CRAFT_RANDOMIZE_H


#include "player/item.h"


#define PROBABILITY_ITEM_NUM 20


typedef void *Randomizable;


typedef struct {
    uint16_t count;
    uint16_t sum;
    struct {
        uint16_t chance;
        Randomizable value;
    } items[PROBABILITY_ITEM_NUM];
} Probability;


Randomizable random_from(Probability *probability);


#endif
