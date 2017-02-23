#include "unit_test.h"
#include "../src/player/inventory.h"
#include "fixture.h"


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

    mu_assert(NULL == inventory->items[0], "The item should be removed");
    mu_assert(NULL == inventory->items[1], "The item should be removed");

    inventory_free(inventory);
}

MU_TEST(test_select)
{
    Inventory *inv = inventory_new(3);

    inventory_select(inv, NORTH);
    mu_assert_int_eq(0, inv->selected);

    inventory_select(inv, SOUTH);
    mu_assert_int_eq(1, inv->selected);

    inventory_select(inv, SOUTH);
    inventory_select(inv, SOUTH);
    mu_assert_int_eq(2, inv->selected);

    inventory_free(inv);
}

MU_TEST(test_use_consumable)
{
    Item item = fixture_consumable(false);
    Inventory inv;
    inv.selected = 0;
    inv.items[0] = &item;
    inv.size = 1;

    Player player;
    Attribute *map = player.attributes.state;
    map[HEALTH].current = 95;
    map[HEALTH].max = 100;

    player.inventory = &inv;

    inventory_use_selected(&player);
    mu_assert_int_eq(100, map[HEALTH].current);
    mu_assert(&item == inv.items[0], "Item should not be removed");

    map[HEALTH].current = 92;
    inventory_use_selected(&player);
    mu_assert_int_eq(97, map[HEALTH].current);
    mu_assert(NULL == inv.items[0], "Item should be removed");
}

void run_inventory_test(void)
{
    TEST_NAME("Inventory");

    MU_RUN_TEST(test_add);
    MU_RUN_TEST(test_remove);
    MU_RUN_TEST(test_select);
    MU_RUN_TEST(test_use_consumable);

    MU_REPORT();
}
