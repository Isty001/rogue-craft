#include "config.h"


AttributeConfig PLAYER_ATTRIBUTE_CONFIG = {
    .fatigue = {
        .time = 300,
        .traveled = 3000,
        .dealt_damage = 1000,
        .thirst = range_new(1, 5),
        .hunger = range_new(1, 6),
        .stamina = range_new(-5, -1)
    },
    .damage = {
        .hunger = 75,
        .thirst = 60,
        .time = 60,
        .health = range_new(1, 2),
        .stamina = range_new(2, 3)
    }
};
