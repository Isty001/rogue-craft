#ifndef ROGUECRAFT_INVENTORY_H
#define ROGUECRAFT_INVENTORY_H


#include "item/item.h"
#include "config.h"
#include "ui/panel.h"
#include "ui/grid.h"


#define inventory_selected(inv) \
    inv->items->get(inv->items, inv->selected)

#define inventory_grid_size(inventory) ceil(sqrt(inventory->max_size))

#define inventory_shortcut_offset(input) (input % INVENTORY_SHORTCUT_FIRST)


typedef struct Inventory {
    uint16_t max_size;
    uint16_t selected;
    List *items;
    Grid *grid;
} Inventory;


Inventory *inventory_new(uint16_t max_size);

ItemError inventory_add(Inventory *inventory, Item *item);

bool inventory_has(Inventory *inventory, Item *item);

ItemError inventory_remove(Inventory *inventory, Item *item);

ItemError inventory_drop_selected(Inventory *inventory, Point around, Level *level);

void inventory_shortcut_display(Inventory *inventory);

void inventory_use_selected(Inventory *inventory, Player *player);

void inventory_grid_update(Inventory *inventory);

EventStatus inventory_player_set_shortcut(PanelInputEvent *event);

EventStatus inventory_close(PanelInputEvent *event);

EventStatus inventory_navigate(PanelInputEvent *event);

EventStatus inventory_display(Inventory *inventory);

EventStatus inventory_player_shortcut_select(InputEvent *event);

EventStatus inventory_player_use_selected(InputEvent *event);

EventStatus inventory_player_display(InputEvent *event);

void inventory_free(Inventory *inventory);


#endif
