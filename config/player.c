#include "config.h"


PlayerStateConfig PLAYER_STATE_CONFIG = {
    .fatigue = {
        .traveled = 3000,
        .dealt_damage = 1000,
        .elapsed_time = 300,
        .thirst = range_new(1, 5),
        .hunger = range_new(1, 6),
        .stamina = range_new(-5, -1)
    },
    .fatigue_damage = {
        .hunger = 75,
        .thirst = 60,
        .elapsed_time = 60,
        .health = range_new(1, 2)
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
