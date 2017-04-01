#ifndef ROGUE_CRAFT_RANDOMIZE_H
#define ROGUE_CRAFT_RANDOMIZE_H


#include <stdint.h>
#include <list.h>


#define PROBABILITY_MAX_ITEM_NUM 30


#define probability_add_to_cache(cache, probability, entry, memb, size, ...)     \
    repeat(probability->count,                                      \
         entry.chance = probability->items[i].chance;               \
         memcpy(&entry.memb, probability->items[i].value, size);    \
         __VA_ARGS__;                                               \
         cache_add(cache, &entry);                                  \
    )


typedef void *Randomizable;


typedef struct {
    uint16_t chance;
    Randomizable value;
} ProbabilityItem;

typedef struct {
    uint16_t count;
    uint16_t sum;
    /** @TODO Dynamic */
    ProbabilityItem items[PROBABILITY_MAX_ITEM_NUM];
} Probability;


Randomizable probability_pick(Probability *probability);

void probability_add(Probability *probability, uint16_t chance, Randomizable item);

void probability_clean(Probability *probability, Release clear);


#endif
