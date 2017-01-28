#include "config.h"
#include "item_config.h"


/**
 * BASE TYPE PROBABILITIES
 */
Probability ITEM_TYPE_PROBABILITY = {
    .size = 1,
    .sum = 50,
    .items = {
        {.value = 50, .ptr = &ITEM_CONSUMABLE_PROBABILITY}
    }
};

Probability ITEM_CONSUMABLE_PROBABILITY = {
    .size = 2,
    .sum = 50,
    .items = {
        {.value = 46, .ptr = &ITEM_POTION_HP},
        {.value = 4, .ptr = &ITEM_POTION_MAX_HP}
    }
};

/**
 * CONSUMABLES
 */
ItemPrototype ITEM_POTION_HP = {
    .data = {
        .name = "HP",
        .description = "Increases the current Health",
        .chr = 'p',
        .color = COLOR_PAIR_RED_F,
        .consumable = {
            .permanent = false,
            .type = HEALTH
        }
    },
    .randomize = item_randomize_consumable
};

ItemPrototype ITEM_POTION_MAX_HP = {
    .data = {
        .name = "Max HP",
        .description = "Increases the maximum Health",
        .chr = 'P',
        .color = COLOR_PAIR_RED_F,
        .consumable = {
            .permanent = true,
            .type = HEALTH
        }
    },
    .randomize = item_randomize_consumable
};

