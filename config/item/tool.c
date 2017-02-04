#include "../config.h"


static ItemPrototype ITEM_TOOL_PICKAXE = {
    .randomize = item_randomize_tool,
    .value_range = {1, 100},
    .item = {
        .name = "",
        .chr = 'p',
        .type = TOOL,
        .style = COLOR_PAIR(COLOR_PAIR_GRAY_F),
        .tool = {
            .damage = {.cell = 5}
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
