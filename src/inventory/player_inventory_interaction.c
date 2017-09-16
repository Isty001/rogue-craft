#include <ui/ncurses.h>
#include <player/player.h>
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

void inventory_player_set_shortcut(PanelInputEvent *event)
{
    const PanelInfo *info = event->info;

    uint16_t taken_from = info->inventory->selected;
    uint16_t replacing_at = inventory_shortcut_offset(event->input);

    Item **from = info->inventory->items;
    Item **to = event->player->inventory->items;

    Item *taken = from[taken_from];
    Item *replacing = to[replacing_at];

//    printf("replacing: %p, taken: %p\n", replacing, taken);

    from[taken_from] = replacing;
    to[replacing_at] = taken;

    inventory_grid_update(event->player->inventory);
}

void inventory_player_drop_shortcut(InputEvent *event)
{
    Player *player = event->player;
    Point around = player->position.current;

    inventory_drop_selected(player->inventory, around, player->level);
}
