#include "../unit_test.h"
#include "../../src/worker/worker.h"


static AttributeConfig TEST_CFG = {
    .fatigue = {
        .time = 5,
        .traveled = 5,
        .dealt_damage = 80,
        .thirst = range_new(5, 5),
        .hunger = range_new(5, 5),
        .stamina = range_new(-5, -5)
    },
    .damage = {
        .hunger = 30,
        .thirst = 30,
        .time = 5,
        .health = range_new(5, 5),
        .stamina = range_new(5, 5)
    }
};

static Player create_player(void)
{
    return (Player) {
        .attributes = {
            .modifiers = {
                .traveled = 6,
                .dealt_damage = 100.0,
                .updated = time(NULL) - 10
            },
            .state = {
                [HEALTH] = {.current = 20},
                [STAMINA] = {.current = 5},
                [HUNGER] = {.current = 20, .max = 30},
                [THIRST] = {.current = 10, .max = 100},
            }
        }
    };
}

MU_TEST(test_state)
{
    Message msg;
    Player player = create_player();

    msg.player_state.player = &player;
    msg.player_state.cfg = &TEST_CFG;

    message_player_state_execute(&msg);

    Modifiers modifiers = player.attributes.modifiers;
    Attribute *state = player.attributes.state;

    mu_assert(
        0 == modifiers.dealt_damage &&
        0 == modifiers.traveled &&
        time(NULL) == modifiers.updated,
        "Invalid modifiers"
    );

    mu_assert(
        30 == state[HUNGER].current &&
        25 == state[THIRST].current,
        "Invalid state"
    );

    mu_assert(
        10 == state[HEALTH].current &&
        0 == state[STAMINA].current,
        "Invalid health/stamina"
    );
}

void run_player_state_test(void)
{
    TEST_NAME("Player state");

    MU_RUN_TEST(test_state);

    MU_REPORT();
}
