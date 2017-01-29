#include "unit_test.h"
#include "../src/player/inventory.h"


MU_TEST(test_add)
{
    Item item, item2, item3;
    Inventory *inventory = inventory_new(2);

    mu_assert_int_eq(IE_OK, inventory_add(inventory, &item));
    mu_assert_int_eq(IE_DUPLICATE, inventory_add(inventory, &item));

    mu_assert_int_eq(IE_OK, inventory_add(inventory, &item2));
    mu_assert_int_eq(IE_OVERFLOW, inventory_add(inventory, &item3));

    mu_assert(inventory->update_display, "Inventory should have changed");

    inventory_free(inventory);
}

void run_inventory_test(void)
{
    TEST_NAME("Inventory");

    MU_RUN_TEST(test_add);

    MU_REPORT();
}
