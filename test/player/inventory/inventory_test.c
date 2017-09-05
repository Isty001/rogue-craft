#include <ui/panel.h>
#include "../../unit_test.h"
#include "player/inventory/inventory.h"
#include "../../fixture.h"


static PanelInputEvent create_panel_event(Inventory *inventory, PanelInfo *info, PanelType type, int input)
{
    inventory->grid = grid_new(NULL, size_new(5, 5), point_new(0, 0));
    info->type = type;
    info->inventory = inventory;

    return (PanelInputEvent) {
        .info = info,
        .input = input
    };
}

MU_TEST(test_close)
{
    Inventory inventory;
    PanelInfo info;
    PanelInputEvent event = create_panel_event(&inventory, &info, -1, 0);

    mu_assert_int_eq(ES_CONTINUE, inventory_close(&event));

    event.info->type = PANEL_INVENTORY;
    mu_assert_int_eq(ES_BREAK, inventory_close(&event));
    mu_assert(NULL == inventory.grid, "");
}

MU_TEST(test_navigation)
{
    PanelInfo info;
    Inventory *inventory = inventory_new(10);;
    PanelInputEvent event = create_panel_event(inventory, &info, -1, 0);

    mu_assert_int_eq(ES_CONTINUE, inventory_navigate(&event));

    event.info->type = PANEL_INVENTORY;
    mu_assert_int_eq(ES_CONTINUE, inventory_navigate(&event));

    event.input = KEY_SOUTH;
    mu_assert_int_eq(ES_BREAK, inventory_navigate(&event));
    assert_point(inventory->grid->selected, 1, 0);

    event.input = KEY_USE;
    mu_assert_int_eq(ES_BREAK, inventory_navigate(&event));

    inventory_free(inventory);
}

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
    player.state.map[HEALTH].current = 95;
    player.state.map[HEALTH].max = 100;

    inventory_player_use_selected(&event);
    mu_assert_int_eq(100, player.state.map[HEALTH].current);
    mu_assert(item == items->head(items), "Item should not be removed");

    player.state.map[HEALTH].current = 92;
    inventory_player_use_selected(&event);
    mu_assert_int_eq(97, player.state.map[HEALTH].current);
    mu_assert(NULL == items->head(items), "Item should be removed");

    inventory_free(inv);
}

MU_TEST(test_select_shortcut)
{
    Inventory *inv = inventory_new(3);
    Player player;
    player.inventory = inv;

    InputEvent event = {.input = '2', .player = &player};

    inventory_player_shortcut_select(&event);
    mu_assert_int_eq(1, inv->selected);

    event.input = INVENTORY_SHORTCUT_FIRST + INVENTORY_SHORTCUT_NUM;
    inventory_player_shortcut_select(&event);
    mu_assert_int_eq(1, inv->selected);

    event.input = 'a';
    inventory_player_shortcut_select(&event);
    mu_assert_int_eq(1, inv->selected);

    inventory_free(inv);
}

MU_TEST(test_set_shortcut)
{
    /* Inventory size has no real meaning at this context. We only replacing two items in two lists */
    Item taken_item;
    Inventory *other = inventory_new(5);

    other->selected = 1;
    other->items->append(other->items, NULL);
    other->items->append(other->items, &taken_item);

    Item player_item;
    Inventory *player_inv = inventory_new(5);
    player_inv->items->append(player_inv->items, NULL);
    player_inv->items->append(player_inv->items, NULL);
    player_inv->items->append(player_inv->items, &player_item);

    Player player;
    player.inventory = player_inv;

    PanelInfo info;
    PanelInputEvent event = create_panel_event(other, &info, -1, KEY_ESCAPE);
    event.player = &player;

    mu_assert_int_eq(ES_CONTINUE, inventory_set_shortcut(&event));

    info.type = PANEL_INVENTORY;
    mu_assert_int_eq(ES_CONTINUE, inventory_set_shortcut(&event));

    event.input = '3';
    mu_assert_int_eq(ES_BREAK, inventory_set_shortcut(&event));

    /* Player's inventory at offset 3 now has the other inventory's selected item */
    mu_assert_int_eq(
        &taken_item,
        player_inv->items->get(player_inv->items, inventory_shortcut_offset(event.input))
    );

    /* The other inventory has the item that was at the player's inventory's offset 3 */
    mu_assert_int_eq(
        &player_item,
        other->items->get(other->items, other->selected)
    );

    /* Remove the item from the shortcut, and put back the original one from the other inventory */
    player_inv->items->delete_at(player_inv->items, inventory_shortcut_offset(event.input));
    mu_assert_int_eq(ES_BREAK, inventory_set_shortcut(&event));

    mu_assert_int_eq(
        NULL,
        other->items->get(other->items, other->selected)
    );

    mu_assert_int_eq(
        &player_item,
        player_inv->items->get(player_inv->items, inventory_shortcut_offset(event.input))
    );

    inventory_free(other);
    inventory_free(player.inventory);
}

void run_inventory_test(void)
{
    TEST_NAME("Inventory");

    MU_RUN_TEST(test_close);
    MU_RUN_TEST(test_navigation);
    MU_RUN_TEST(test_add);
    MU_RUN_TEST(test_remove);
    MU_RUN_TEST(test_use_consumable);
    MU_RUN_TEST(test_select_shortcut);
    MU_RUN_TEST(test_set_shortcut);

    MU_REPORT();
    exit(0);
}
