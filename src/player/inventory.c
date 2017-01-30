#include "inventory.h"


Inventory *inventory_new(uint16_t size)
{
    Inventory *inventory = alloc(sizeof(Inventory) + (size * sizeof(Item *)));
    inventory->update_display = true;
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
               inventory->update_display = true;

               return IE_OK;
           }
    )
    return IE_OVERFLOW;
}

void inventory_display(Inventory *inventory)
{
    if (!inventory->update_display) {
        return;
    }
    Item *item;

    wclear(WINDOW_INVENTORY);

    for (int i = 0; i < inventory->size; i++) {
        wmove(WINDOW_INVENTORY, i + 1, 2);

        if ((item = inventory->items[i])) {
            styled(WINDOW_INVENTORY, item->style,
                   wprintw(WINDOW_INVENTORY, "%lc", item->chr);
            );
            wprintw(WINDOW_INVENTORY, " | %d | %s", item->value, item->name);
        } else {
            waddch(WINDOW_INVENTORY, '-');
        }
    }
    refresh_boxed(WINDOW_INVENTORY);

    inventory->update_display = false;
}

void inventory_free(Inventory *inventory)
{
    free(inventory);
}
