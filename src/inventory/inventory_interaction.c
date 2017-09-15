#include <ui/panel.h>
#include "inventory.h"


void inventory_close(PanelInputEvent *event)
{
    const PanelInfo *info = event->info;

    Inventory *inventory = info->inventory;

    grid_free(inventory->grid);
    inventory->grid = NULL;
}

static uint16_t selected_item(Grid *grid, Inventory *inventory)
{
    uint16_t size = inventory_grid_size(inventory);
    Point selected = grid->selected;

    return min(inventory->max_size, (selected.y * size) + selected.x);
}

static void move_selected(Inventory *inventory, int input)
{
    Grid *grid = inventory->grid;
    Direction direction = direction_lookup(input);

    if (direction) {
        grid_move_selected(grid, direction);
        inventory->selected = selected_item(grid, inventory);
    }
}

void inventory_navigate(PanelInputEvent *event)
{
    Inventory *inventory = event->info->inventory;
    int input = event->input;

    if (KEY_USE_SELECTED == input) {
        inventory_use_selected(inventory, event->player);
    } else {
        move_selected(inventory, input);
    }
    inventory_grid_update(inventory);
}

void inventory_use_selected(Inventory *inventory, Player *player)
{
    Item *selected = inventory_selected(inventory);

    if (selected) {
        if (CONSUMABLE == selected->type) {
            if (IE_CONSUMED == item_consume(selected, player)) {
                inventory_remove(inventory, selected);
                item_free(selected);
            }
        }
    }
}

