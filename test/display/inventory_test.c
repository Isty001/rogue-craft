#include "../unit_test.h"
#include "../../src/player/inventory.h"


static PanelEvent create_event(Inventory *inventory, PanelInfo *info, PanelType type, int input)
{
    inventory->grid = grid_new(NULL, size_new(5, 5), point_new(0, 0));
    info->type = type;
    info->inventory = inventory;

    return (PanelEvent) {
        .info = info,
        .input = input
    };
}

MU_TEST(test_close)
{
    Inventory inventory;
    PanelInfo info;
    PanelEvent event = create_event(&inventory, &info, -1, 0);

    mu_assert_int_eq(EE_CONTINUE, inventory_close(&event));

    event.info->type = INVENTORY;
    mu_assert_int_eq(EE_BREAK, inventory_close(&event));
    mu_assert(NULL == inventory.grid, "");
}

MU_TEST(test_navigation)
{
    PanelInfo info;
    Inventory inventory;
    inventory.max_size = 10;
    inventory.items = list_new();
    PanelEvent event = create_event(&inventory, &info, -1, 0);

    mu_assert_int_eq(EE_CONTINUE, inventory_navigate(&event));

    event.info->type = INVENTORY;
    mu_assert_int_eq(EE_CONTINUE, inventory_navigate(&event));

    event.input = KEY_SOUTH;
    mu_assert_int_eq(EE_BREAK, inventory_navigate(&event));
    assert_point(inventory.grid->selected, 1, 0);

    event.input = KEY_USE;
    mu_assert_int_eq(EE_BREAK, inventory_navigate(&event));

    inventory.items->free(inventory.items);
    grid_free(inventory.grid);
}

void run_inventory_display_test(void)
{
    TEST_NAME("Inventory Display");

    MU_RUN_TEST(test_close);
    MU_RUN_TEST(test_navigation);

    MU_REPORT();
}
