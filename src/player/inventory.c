#include "item.h"


Inventory *inventory_new(uint16_t size)
{
    Inventory *inventory = alloc(sizeof(Inventory) + (size * sizeof(Item *)));
    inventory->size = size;
    repeat(inventory->size, inventory->items[i] = NULL)

    return inventory;
}

static inline bool in_inventory(Inventory *inventory, Item *item)
{
    repeat(inventory->size,
        if (inventory->items[i] == item) return true;
    )
    return false;
}

ItemError inventory_add(Inventory *inventory, Item *item)
{
    if (in_inventory(inventory, item)) return IE_DUPLICATE;

    repeat(inventory->size,
        if (NULL == inventory->items[i]) {
            inventory->items[i] = item;

            return IE_OK;
        }
    )
    return IE_OVERFLOW;
}

void inventory_free(Inventory *inventory)
{
    free(inventory);
}

void inventory_interact(Player *player, Click *click)
{
}

void inventory_display(Inventory *inventory)
{
}
