#include "config.h"


const PlayerStateConfig PLAYER_STATE_CONFIG = {
    .fatigue = {
        .traveled = 1000,
        .dealt_damage = 500,
        .elapsed_time = 100,
        .thirst = range_new(1, 5),
        .hunger = range_new(1, 6),
        .stamina = range_new(-5, -1)
    },
    .fatigue_damage = {
        .hunger = 75,
        .thirst = 60,
        .elapsed_time = 60,
        .health = range_new(-2, -1)
    }
};

