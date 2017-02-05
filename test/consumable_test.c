#include "unit_test.h"
#include "../src/player/item.h"
#include "fixture.h"


MU_TEST(test_non_consumable)
{
    Item item;
    item.type = TOOL;

    mu_assert(IE_INVALID_ARGUMENT == item_consume(&item, NULL), "Should give Invalid Arg");
}

static void setup_player(Player *player)
{
    player->attributes[HEALTH].increase_max = 100;
    player->attributes[HEALTH].modification_limit = 100;
    player->attributes[HEALTH].current = 90;
    player->attributes[HEALTH].increasing = false;

    player->attributes[HUNGER].modification_limit = 0;
    player->attributes[HUNGER].increase_max = 100;
    player->attributes[HUNGER].current = 20;
    player->attributes[HUNGER].increasing = true;
}

MU_TEST(test_persistent)
{
    Item item = fixture_consumable(true);

    Player player;
    setup_player(&player);

    mu_assert(IE_CONSUMED == item_consume(&item, &player), "Item should be consumed");
    mu_assert_int_eq(110, player.attributes[HEALTH].increase_max);
}

MU_TEST(test_non_persistent)
{
    Item item = fixture_consumable(false);
    Player player;
    setup_player(&player);

    mu_assert_int_eq(IE_CONSUMED, item_consume(&item, &player));
    mu_assert_int_eq(100, player.attributes[HEALTH].current);
}

MU_TEST(test_partial_consume)
{
    Item item = fixture_consumable(false);
    Player player;
    setup_player(&player);
    player.attributes[HEALTH].current = 95;

    mu_assert(IE_REPEAT == item_consume(&item, &player), "Should be partially consumed");
    mu_assert_int_eq(100, player.attributes[HEALTH].current);
    mu_assert_int_eq(5, item.value);
}

MU_TEST(test_negative)
{
    Player player;
    setup_player(&player);

    Item item = fixture_consumable(false);
    item.consumable.type = HUNGER;
    item.value = -10;

    mu_assert_int_eq(IE_CONSUMED, item_consume(&item, &player));

    mu_assert_int_eq(10, player.attributes[HUNGER].current);
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
