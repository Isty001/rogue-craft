#ifndef ROGUE_CRAFT_RANDOMIZE_H
#define ROGUE_CRAFT_RANDOMIZE_H


#include <stdint.h>
#include <list.h>


#define PROBABILITY_MAX_ITEM_NUM 50


#define probability_add_to_cache(cache, probability, entry, memb, size, ...)     \
    repeat(probability->count,                                      \
         entry.chance = probability->items[i].chance;               \
         memcpy(&entry.memb, probability->items[i].value, size);    \
         __VA_ARGS__;                                               \
         cache_add(cache, &entry);                                  \
    )


typedef struct {
    uint16_t chance;
    const void *value;
} ProbabilityItem;

typedef struct {
    uint16_t count;
    uint16_t sum;
    ProbabilityItem items[PROBABILITY_MAX_ITEM_NUM];
} Probability;


void *probability_pick(const Probability *probability);

void probability_add(Probability *probability, uint16_t chance, const void *item);

void probability_clean(Probability *probability, Release clear);


#endif
