#ifndef ROGUECRAFT_INVENTORY_H
#define ROGUECRAFT_INVENTORY_H


#include "item.h"



typedef struct Inventory {
    bool update_display;
    uint16_t size;
    Item *items[];
} Inventory;


Inventory *inventory_new(uint16_t size);

ItemError inventory_add(Inventory *inventory, Item *item);

void inventory_display(Inventory *inventory);

void inventory_free(Inventory *inventory);


#endif
