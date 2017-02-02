#include "inventory.h"


void inventory_display(Inventory *inventory)
{
    if (!inventory->update_display) {
        return;
    }
    Item *item;

    wclear(WINDOW_INVENTORY);

    for (int i = 0; i < inventory->size; i++) {
        wmove(WINDOW_INVENTORY, i + PADDING, PADDING);

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
