#include "../unit_test.h"
#include "../../src/player/player.h"


#define attr_init(curr) (State){.current = curr, .max = 100}


static const PlayerStateConfig TEST_CFG = {
    .fatigue = {
        .hunger = range_new(1, 1),
        .thirst = range_new(1, 1),
        .stamina = range_new(-1, -1),
        .traveled = 10,
        .elapsed_time = 10,
        .dealt_damage = 10
    },
    .fatigue_damage = {
        .health = range_new(-1, -1),
        .hunger = 80,
        .thirst = 90,
        .elapsed_time = 10
    }
};


MU_TEST(test_player_state)
{
    Player player;
    State *state = player.state.map;
    state[HEALTH] = attr_init(100);
    state[HUNGER] = attr_init(98);
    state[STAMINA] = attr_init(2);
    state[THIRST] = attr_init(97);

    Modifiers *modifiers = &player.state.modifiers;
    *modifiers = (Modifiers) {
        .traveled = 10,
        .dealt_damage = 14,
        .timestamp = {
            .fatigue = time(NULL) - 10,
            .fatigue_damage = time(NULL) - 12
        }
    };

    TimerArgs args = {.ptr = {&player, &TEST_CFG}};
    player_state_update(&args);

    mu_assert_int_eq(100, state[HUNGER].current);
    mu_assert_int_eq(0, state[STAMINA].current);
    mu_assert_int_eq(100, state[THIRST].current);

    mu_assert_int_eq(98, state[HEALTH].current);

    mu_assert_int_eq(0, modifiers->traveled);
    mu_assert_int_eq(0, modifiers->dealt_damage);
    mu_assert_int_eq(time(NULL), modifiers->timestamp.fatigue);
    mu_assert_int_eq(time(NULL), modifiers->timestamp.fatigue_damage);
}

void run_player_state_test(void)
{
    TEST_NAME("Player State");

    MU_RUN_TEST(test_player_state);

    MU_REPORT();
}
