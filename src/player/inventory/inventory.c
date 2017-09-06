#include "memory/memory.h"
#include "inventory.h"


Inventory *inventory_new(uint16_t max_size)
{
    Inventory *inventory = mem_alloc(sizeof(Inventory));
    inventory->max_size = max_size;
    inventory->selected = 0;
    inventory->grid = NULL;
    inventory->items = list_new();

    return inventory;
}

void inventory_free(Inventory *inventory)
{
    mem_dealloc(inventory->items);

    if (inventory->grid) {
        grid_free(inventory->grid);
    }

    mem_dealloc(inventory);
}

static inline bool in_inventory(Inventory *inventory, Item *item)
{
    List *items = inventory->items;

    return items->has(items, item);
}

ItemError inventory_add(Inventory *inventory, Item *item)
{
    if (in_inventory(inventory, item)) {
        return IE_DUPLICATE;
    }
    List *items = inventory->items;

    if (inventory->max_size == items->count) {
        ncurses_event("Your Inventory is full");
        return IE_OVERFLOW;
    }
    items->append(items, item);
    ncurses_event("%s added to your inventory", item->name);

    return IE_OK;
}

ItemError inventory_remove(Inventory *inventory, Item *item)
{
    if (!in_inventory(inventory, item)) {
        return IE_INVALID_ARGUMENT;
    }
    List *items = inventory->items;

    items->delete(items, item);
    wclear(WINDOW_INVENTORY_SHORTCUT);

    return IE_OK;
}
