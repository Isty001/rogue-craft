#include "randomization.h"
#include "util/util.h"


Randomizable probability_pick(Probability *probability)
{
    uint16_t picked = (uint16_t) rand_in(0, probability->sum);
    uint16_t cumulative = 0;

    for (int i = 0; i < probability->count; i++) {
        cumulative += probability->items[i].chance;

        if (picked <= cumulative) {
            return probability->items[i].value;
        }
    }
    fatal("Unable to pick random value");
}

void probability_add(Probability *probability, uint16_t chance, Randomizable item)
{
    probability->items[probability->count].chance = chance;
    probability->items[probability->count].value = item;
    probability->sum += chance;
    probability->count++;
}

void probability_clean(Probability *probability, Release clear)
{
    for (int i = 0; i < probability->count; i++) {
        clear(probability->items[i].value);
    }
    probability->count = 0;
    probability->sum = 0;
}
