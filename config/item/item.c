#include "../config.h"


/**
 * BASE TYPE PROBABILITIES
 */
Probability ITEM_TYPE_PROBABILITY = {
    .count = 2,
    .sum = 60,
    .items = {
        {.chance = 50, .value = &ITEM_CONSUMABLE_PROBABILITY},
        {.chance = 10, .value = &ITEM_TOOL_PROBABILITY}
    }
};

