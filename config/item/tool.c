#include "../config.h"


static ItemPrototype ITEM_TOOL_PICKAXE = {
    .randomize = item_randomize_value,
    .value_range = {1, 100},
    .item = {
        .name = "Pickaxe",
        .chr = 'p',
        .type = TOOL,
        .style = COLOR_PAIR(COLOR_PAIR_GRAY_F),
        .tool = {
            .range = 1,
            .multipliers = {
                .defaults = {.solid = 1.0, .creature = 1.0},
                .materials = {
                    [STONE] = 1.4,
                    [DIRT] = 1.1
                }
            }
        }
    }
};


Probability ITEM_TOOL_PROBABILITY = {
    .count = 1,
    .sum = 10,
    .items = {
        {.chance = 10, .value = &ITEM_TOOL_PICKAXE}
    }
};
