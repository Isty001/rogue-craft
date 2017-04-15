#ifndef ROGUECRAFT_INVENTORY_H
#define ROGUECRAFT_INVENTORY_H


#include "item.h"
#include "../../config/config.h"
#include "../ncurses/panel.h"
#include "../ncurses/grid.h"


#define inventory_selected(inv) \
    inv->items->get(inv->items, inv->selected)

#define inventory_grid_size(inventory) ceil(sqrt(inventory->max_size))

#define inventory_is_shortcut(input) \
    (input >= INVENTORY_SHORTCUT_FIRST && input < INVENTORY_SHORTCUT_FIRST + INVENTORY_SHORTCUT_NUM)


typedef struct Inventory {
    uint16_t max_size;
    uint16_t selected;
    List *items;
    Grid *grid;
} Inventory;


Inventory *inventory_new(uint16_t max_size);

ItemError inventory_add(Inventory *inventory, Item *item);

ItemError inventory_remove(Inventory *inventory, Item *item);

EventError inventory_player_shortcut_select(InputEvent *event);

void inventory_shortcut_display(Player *player);

void inventory_use_selected(Inventory *inventory, Player *player);

EventError inventory_player_use_selected(InputEvent *event);

void inventory_grid_update(Inventory *inventory);

EventError inventory_player_display(InputEvent *event);

EventError inventory_close(PanelEvent *event);

EventError inventory_navigate(PanelEvent *event);

void inventory_free(Inventory *inventory);


#endif
