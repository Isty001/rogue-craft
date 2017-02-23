#include "../unit_test.h"
#include "../../src/worker/worker.h"


/** Because of -Wignored-qualifiers */

#define create_change(lim) {.limit = lim, .hunger = range_new(10, 10), .thirst = range_new(20, 20), .stamina = range_new(-5, -5)}

#define create_attr_cfg {                                                                       \
    .traveled = create_change(5), .time = create_change(5), .dealt_damage = create_change(70),  \
    .damage = {.hunger_limit = 30, .thirst_limit = 70}                                          \
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
                [HEALTH] = {.current = 10},
                [STAMINA] = {.current = 2},
                [HUNGER] = {.current = 20, .max = 40},
                [THIRST] = {.current = 10, .max = 100},
            }
        }
    };
}

MU_TEST(test_state)
{
    Message msg;
    Player player = create_player();
    AttributeConfig cfg = create_attr_cfg;

    msg.player_state.player = &player;
    msg.player_state.cfg = &cfg;

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
        40 == state[HUNGER].current &&
        70 == state[THIRST].current,
        "Invalid state"
    );
}

void run_player_state_test(void)
{
    TEST_NAME("Player state");

    MU_RUN_TEST(test_state);

    MU_REPORT();
}
