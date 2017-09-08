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

EventStatus inventory_player_use_selected(InputEvent *event)
{
    if (KEY_USE != event->input) {
        return ES_CONTINUE;
    }

    Inventory *inventory = event->player->inventory;
    inventory_use_selected(inventory, event->player);

    return ES_BREAK;
}

static void assure_list_size(List *items, uint16_t until)
{
    while (until >= items->count) {
        items->append(items, NULL);
    }
}

EventStatus inventory_player_set_shortcut(PanelInputEvent *event)
{
    PanelInfo *info = event->info;

    if (PANEL_INVENTORY != info->type || !is_shortcut(event->input)) {
        return ES_CONTINUE;
    }

    uint16_t taken_from = info->inventory->selected;
    uint16_t replacing_at = inventory_shortcut_offset(event->input);

    List *from = info->inventory->items;
    List *to = event->player->inventory->items;

    assure_list_size(to, replacing_at);

    Item *taken = from->get(from, taken_from);
    Item *replacing = to->get(to, replacing_at);

    from->set(from, taken_from, replacing);
    to->set(to, replacing_at, taken);

    inventory_grid_update(event->player->inventory);

    return ES_BREAK;
}
