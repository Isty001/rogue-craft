#include "unit_test.h"
#include "../src/player/inventory.h"
#include "fixture.h"
#include "../src/ncurses/ncurses.h"
#include "../config/config.h"


MU_TEST(test_add)
{
    Item item, item2, item3;
    Inventory *inventory = inventory_new(2);

    mu_assert_int_eq(IE_OK, inventory_add(inventory, &item));
    mu_assert_int_eq(IE_DUPLICATE, inventory_add(inventory, &item));

    mu_assert_int_eq(IE_OK, inventory_add(inventory, &item2));
    mu_assert_int_eq(IE_OVERFLOW, inventory_add(inventory, &item3));

    inventory_free(inventory);
}

MU_TEST(test_remove)
{
    Item item, item2;
    Inventory *inventory = inventory_new(2);

    inventory_add(inventory, &item);
    inventory_add(inventory, &item2);

    mu_assert_int_eq(IE_OK, inventory_remove(inventory, &item));
    mu_assert_int_eq(IE_INVALID_ARGUMENT, inventory_remove(inventory, &item));

    mu_assert_int_eq(IE_OK, inventory_remove(inventory, &item2));

    mu_assert(NULL == inventory->items[0], "The items should be removed");
    mu_assert(NULL == inventory->items[1], "The items should be removed");

    inventory_free(inventory);
}

MU_TEST(test_select)
{
    Inventory *inv = inventory_new(3);
    Player player;
    player.inventory = inv;

    InputEvent event = {.input = '2', .player = &player};

    inventory_shortcut_select(&event);
    mu_assert_int_eq(1, inv->selected);

    event.input = INVENTORY_SHORTCUT_FIRST + INVENTORY_SHORTCUT_NUM;
    inventory_shortcut_select(&event);
    mu_assert_int_eq(1, inv->selected);

    event.input = 'a';
    inventory_shortcut_select(&event);
    mu_assert_int_eq(1, inv->selected);

    inventory_free(inv);
}

MU_TEST(test_use_consumable)
{
    Item item = fixture_consumable(false);
    Inventory *inv = inventory_new(1);
    inventory_add(inv, &item);

    Player player;

    player.inventory = inv;
    player.attributes.state[HEALTH].current = 95;
    player.attributes.state[HEALTH].max = 100;

    inventory_use_selected(&player);
    mu_assert_int_eq(100, player.attributes.state[HEALTH].current);
    mu_assert(&item == inv->items[0], "Item should not be removed");

    player.attributes.state[HEALTH].current = 92;
    inventory_use_selected(&player);
    mu_assert_int_eq(97, player.attributes.state[HEALTH].current);
    mu_assert(NULL == inv->items[0], "Item should be removed");

    inventory_free(inv);
}

void run_inventory_test(void)
{
    TEST_NAME("Inventory");

    MU_RUN_TEST(test_use_consumable);
    MU_RUN_TEST(test_add);
    MU_RUN_TEST(test_select);
    MU_RUN_TEST(test_remove);

    MU_REPORT();
}
