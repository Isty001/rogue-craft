#ifndef ROGUECRAFT_INVENTORY_H
#define ROGUECRAFT_INVENTORY_H


#include "item.h"


typedef struct Inventory {
    uint16_t size;
    uint16_t selected;
    Item *items[];
} Inventory;


Inventory *inventory_new(uint16_t size);

ItemError inventory_add(Inventory *inventory, Item *item);

ItemError inventory_remove(Inventory *inventory, Item *item);

void inventory_select(Inventory *inventory, Direction direction);

void inventory_use_selected(Player *player);

void inventory_display(Inventory *inventory);

void inventory_free(Inventory *inventory);


#endif
