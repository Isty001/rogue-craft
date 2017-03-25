#include "config.h"


AttributeConfig PLAYER_ATTRIBUTE_CONFIG = {
    .fatigue = {
        .traveled = 3000,
        .dealt_damage = 1000,
        .time_limit = 300,
        .thirst = range_new(1, 5),
        .hunger = range_new(1, 6),
        .stamina = range_new(-5, -1)
    },
    .damage = {
        .hunger_limit = 75,
        .thirst_limit = 60,
        .time_limit = 60,
        .health = range_new(1, 2),
        .stamina = range_new(2, 3)
    }
};

Probability ITEM_TYPE_PROBABILITY = {
    .count = 2,
    .sum = 60,
    .items = {
        {.chance = 50, .value = &ITEM_CONSUMABLE_PROBABILITY},
        {.chance = 10, .value = &ITEM_TOOL_PROBABILITY}
    }
};
