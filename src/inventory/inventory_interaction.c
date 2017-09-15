#include <ui/panel.h>
#include "inventory.h"


EventStatus inventory_close(PanelInputEvent *event)
{
    PanelInfo *info = event->info;

    if (PANEL_INVENTORY != info->type) {
        return ES_CONTINUE;
    }
    Inventory *inventory = info->inventory;

    grid_free(inventory->grid);
    inventory->grid = NULL;

    return ES_BREAK;
}

static uint16_t selected_item(Grid *grid, Inventory *inventory)
{
    uint16_t size = inventory_grid_size(inventory);
    Point selected = grid->selected;

    return min(inventory->max_size, (selected.y * size) + selected.x);
}

static EventStatus move_selected(Inventory *inventory, int input)
{
    Grid *grid = inventory->grid;
    Direction direction = direction_lookup(input);

    if (direction) {
        grid_move_selected(grid, direction);
        inventory->selected = selected_item(grid, inventory);

        return ES_BREAK;
    }

    return ES_CONTINUE;
}

EventStatus inventory_navigate(PanelInputEvent *event)
{
    if (PANEL_INVENTORY != event->info->type) {
        return ES_CONTINUE;
    }

    EventStatus err;
    Inventory *inventory = event->info->inventory;
    int input = event->input;

    err = move_selected(inventory, input);

    if (KEY_USE == input) {
        inventory_use_selected(inventory, event->player);

        err = ES_BREAK;
    }
    inventory_grid_update(inventory);

    return err;
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

