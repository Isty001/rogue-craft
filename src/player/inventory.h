#ifndef ROGUECRAFT_INVENTORY_H
#define ROGUECRAFT_INVENTORY_H


#include "item.h"
#include "../../config/config.h"


#define inventory_selected(inv) \
    inv->items->get(inv->items, inv->selected)


typedef struct Inventory {
    uint16_t max_size;
    uint16_t selected;
    List *items;
    Item **on_shortcut[INVENTORY_SHORTCUT_NUM];
} Inventory;


Inventory *inventory_new(uint16_t max_size);

ItemError inventory_add(Inventory *inventory, Item *item);

ItemError inventory_remove(Inventory *inventory, Item *item);

EventError inventory_shortcut_select(InputEvent *event);

EventError inventory_use_selected(InputEvent *event);

void inventory_shortcut_display(Inventory *inventory);

void inventory_free(Inventory *inventory);


#endif
