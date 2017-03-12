#ifndef ROGUECRAFT_INVENTORY_H
#define ROGUECRAFT_INVENTORY_H


#include "item.h"
#include "../../config/config.h"


#define inventory_selected(inventory) \
    inventory->items[inventory->selected]


typedef struct Inventory {
    uint16_t size;
    uint16_t selected;
    Item **on_shortcut[INVENTORY_SHORTCUT_NUM];
    Item *items[];
} Inventory;


Inventory *inventory_new(uint16_t size);

ItemError inventory_add(Inventory *inventory, Item *item);

ItemError inventory_remove(Inventory *inventory, Item *item);

EventError inventory_shortcut_select(InputEvent *event);

void inventory_use_selected(Player *player);

void inventory_shortcut_display(Inventory *inventory);

void inventory_free(Inventory *inventory);


#endif
