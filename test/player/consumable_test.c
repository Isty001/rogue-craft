#include "../unit_test.h"
#include "item/item.h"
#include "../fixture.h"


MU_TEST(test_non_consumable)
{
    Item item;
    item.type = ITEM_TOOL;

    mu_assert(IE_INVALID_ARGUMENT == item_consume(&item, NULL), "Should give Invalid Arg");
}

static void setup_player(Player *player)
{
    State *states = player->state.map;

    states[STATE_HEALTH].max = 100;
    states[STATE_HEALTH].current = 90;

    states[STATE_HUNGER].max = 100;
    states[STATE_HUNGER].current = 20;
}

MU_TEST(test_persistent)
{
    Item item = fixture_consumable(true);

    Player player;
    setup_player(&player);

    mu_assert(IE_CONSUMED == item_consume(&item, &player), "Item should be consumed");
    mu_assert_int_eq(110, player.state.map[STATE_HEALTH].max);
}

MU_TEST(test_non_persistent)
{
    Item item = fixture_consumable(false);
    Player player;
    setup_player(&player);

    mu_assert_int_eq(IE_CONSUMED, item_consume(&item, &player));
    mu_assert_int_eq(100, player.state.map[STATE_HEALTH].current);
}

MU_TEST(test_partial_consume)
{
    Item item = fixture_consumable(false);
    Player player;
    setup_player(&player);
    player.state.map[STATE_HEALTH].current = 95;

    mu_assert(IE_CAN_BE_CONSUMED == item_consume(&item, &player), "Should be partially consumed");
    mu_assert_int_eq(100, player.state.map[STATE_HEALTH].current);
    mu_assert_int_eq(5, item.value);
}

MU_TEST(test_negative)
{
    Player player;
    setup_player(&player);

    Item item = fixture_consumable(false);
    item.consumable.state_type = STATE_HUNGER;
    item.value = -50;

    mu_assert_int_eq(IE_CAN_BE_CONSUMED, item_consume(&item, &player));
    mu_assert_int_eq(0, player.state.map[STATE_HUNGER].current);

    mu_assert_int_eq(-30, item.value);
}

void run_consumable_test(void)
{
    TEST_NAME("Consumable Item");

    MU_RUN_TEST(test_non_consumable);
    MU_RUN_TEST(test_persistent);
    MU_RUN_TEST(test_non_persistent);
    MU_RUN_TEST(test_partial_consume);
    MU_RUN_TEST(test_negative);

    MU_REPORT();
}
