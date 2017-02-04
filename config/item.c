#include "config.h"


/**
 * BASE TYPE PROBABILITIES
 */
Probability ITEM_TYPE_PROBABILITY = {
    .size = 1,
    .sum = 50,
    .items = {
        {.chance = 50, .value = &ITEM_CONSUMABLE_PROBABILITY}
    }
};

