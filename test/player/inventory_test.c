#include "../unit_test.h"
#include "../../src/player/inventory.h"
#include "../fixture.h"


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
    List *items = inventory->items;

    inventory_add(inventory, &item);
    inventory_add(inventory, &item2);

    mu_assert_int_eq(IE_OK, inventory_remove(inventory, &item));
    mu_assert_int_eq(IE_INVALID_ARGUMENT, inventory_remove(inventory, &item));

    mu_assert_int_eq(IE_OK, inventory_remove(inventory, &item2));

    mu_assert(NULL == items->head(items), "The items should be removed");
    mu_assert(NULL == items->get(items, 1), "The items should be removed");

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
    Item *item = item_allocate();
    *item = fixture_consumable(false);

    Inventory *inv = inventory_new(1);
    List *items = inv->items;
    inventory_add(inv, item);

    Player player;
    InputEvent event = {
        .input = KEY_USE,
        .player = &player
    };

    player.inventory = inv;
    player.state.attributes[HEALTH].current = 95;
    player.state.attributes[HEALTH].max = 100;

    inventory_use_selected(&event);
    mu_assert_int_eq(100, player.state.attributes[HEALTH].current);
    mu_assert(item == items->head(items), "Item should not be removed");

    player.state.attributes[HEALTH].current = 92;
    inventory_use_selected(&event);
    mu_assert_int_eq(97, player.state.attributes[HEALTH].current);
    mu_assert(NULL == items->head(items), "Item should be removed");

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
