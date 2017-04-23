#include "../unit_test.h"
#include "../../src/player/player.h"


#define attr_init(curr) (Attribute){.current = curr, .max = 100}


static PlayerStateConfig TEST_CFG = {
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
    Attribute *attr = player.state.attributes;
    attr[HEALTH] = attr_init(100);
    attr[HUNGER] = attr_init(98);
    attr[STAMINA] = attr_init(2);
    attr[THIRST] = attr_init(97);

    Modifiers *modifiers = &player.state.modifiers;
    *modifiers = (Modifiers) {
        .traveled = 10,
        .dealt_damage = 14,
        .timestamp = {
            .fatigue = time(NULL) - 10,
            .fatigue_damage = time(NULL) - 12
        }
    };
    player_state_update(&player, &TEST_CFG);

    mu_assert_int_eq(100, attr[HUNGER].current);
    mu_assert_int_eq(0, attr[STAMINA].current);
    mu_assert_int_eq(100, attr[THIRST].current);

    mu_assert_int_eq(98, attr[HEALTH].current);

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
