#include "../config.h"


static ItemPrototype ITEM_TOOL_PICKAXE = {
    .randomize = item_randomize_tool,
    .value_range = {1, 100},
    .item = {
        .name = "Pickaxe",
        .chr = 'p',
        .type = TOOL,
        .style = COLOR_PAIR(COLOR_PAIR_GRAY_F),
        .tool = {
            .range = 1,
            .damage = {
                .defaults = {.solid = 10, .creature = 1},
                .materials = {
                    [STONE] = 20,
                    [DIRT] = 15
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
