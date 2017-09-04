#include <ui/panel.h>
#include "inventory.h"


#define is_shortcut(input) \
    (input >= INVENTORY_SHORTCUT_FIRST && input < INVENTORY_SHORTCUT_FIRST + INVENTORY_SHORTCUT_NUM)


EventStatus inventory_player_display(InputEvent *event)
{
    Inventory *inventory = event->player->inventory;

    if (KEY_PLAYER_INVENTORY != event->input) {
        return ES_CONTINUE;
    }

    return inventory_display(inventory);
}

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

EventStatus inventory_player_shortcut_select(InputEvent *event)
{
    int input = event->input;

    if (!is_shortcut(input)) {
        return ES_CONTINUE;
    }

    Inventory *inventory = event->player->inventory;
    uint16_t selected = inventory_shortcut_offset(input);

    if (selected <= INVENTORY_SHORTCUT_NUM) {
        inventory->selected = selected;
    }

    return ES_BREAK;
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

EventStatus inventory_player_use_selected(InputEvent *event)
{
    if (KEY_USE != event->input) {
        return ES_CONTINUE;
    }

    Inventory *inventory = event->player->inventory;

    inventory_use_selected(inventory, event->player);

    return ES_BREAK;
}

EventStatus inventory_set_shortcut(PanelInputEvent *event)
{
    PanelInfo *info = event->info;

    if (PANEL_INVENTORY != info->type || !is_shortcut(event->input)) {
        return ES_CONTINUE;
    }

    uint16_t taken_from = info->inventory->selected;
    uint16_t replacing_at = inventory_shortcut_offset(event->input);

    List *from = info->inventory->items;
    List *to = event->player->inventory->items;

    Item *taken = from->get(from, taken_from);
    Item *replacing = to->get(to, replacing_at);

    from->set(from, taken_from, replacing);
    to->set(to, replacing_at, taken);

    return ES_BREAK;
}
