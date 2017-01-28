#include "config.h"
#include "item_config.h"
#include "../src/player/randomize.h"


ItemProbability ITEM_CONSUMABLE_PROBABILITY = {
    .size = 2,
    .all = 50,
    .probabilities = {
        {.value = 46, .item = &ITEM_POTION_HP},
        {.value = 4, .item = &ITEM_POTION_MAX_HP}
    }
};

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

