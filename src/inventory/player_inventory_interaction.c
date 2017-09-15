#include <ui/ncurses.h>
#include "inventory.h"


void inventory_player_display(InputEvent *event)
{
    (void) event;

    inventory_display(event->player->inventory);
}

void inventory_player_shortcut_select(InputEvent *event)
{
    int input = event->input;

    Inventory *inventory = event->player->inventory;
    uint16_t selected = inventory_shortcut_offset(input);

    if (selected <= INVENTORY_SHORTCUT_NUM) {
        inventory->selected = selected;
    }
}

void inventory_player_use_selected(InputEvent *event)
{
    Inventory *inventory = event->player->inventory;
    inventory_use_selected(inventory, event->player);
}

static void assure_list_size(List *items, uint16_t until)
{
    while (until >= items->count) {
        items->append(items, NULL);
    }
}

void inventory_player_set_shortcut(PanelInputEvent *event)
{
    const PanelInfo *info = event->info;

    uint16_t taken_from = info->inventory->selected;
    uint16_t replacing_at = inventory_shortcut_offset(event->input);

    List *from = info->inventory->items;
    List *to = event->player->inventory->items;

    // Offset starts from 0
    assure_list_size(to, replacing_at + 1);

    Item *taken = from->get(from, taken_from);
    Item *replacing = to->get(to, replacing_at);

    from->set(from, taken_from, replacing);
    to->set(to, replacing_at, taken);

    inventory_grid_update(event->player->inventory);
}
