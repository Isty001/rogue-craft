#include "inventory.h"


Inventory *inventory_new(uint16_t size)
{
    Inventory *inventory = alloc(sizeof(Inventory) + (size * sizeof(Item *)));
    inventory->size = size;
    inventory->selected = 0;
    repeat(inventory->size, inventory->items[i] = NULL)

    return inventory;
}

void inventory_free(Inventory *inventory)
{
    free(inventory);
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
    if (in_inventory(inventory, item)) {
        event("Something went wrong. The exact same item is already in your Inventory");

        return IE_DUPLICATE;
    }

    repeat(inventory->size,
           if (NULL == inventory->items[i]) {
               inventory->items[i] = item;
               event("Picked up %s", item->name);

               return IE_OK;
           }
    )
    event("Your Inventory is full");

    return IE_OVERFLOW;
}

ItemError inventory_remove(Inventory *inventory, Item *item)
{
    if (!in_inventory(inventory, item)) {
        return IE_INVALID_ARGUMENT;
    }

    repeat(inventory->size,
           if (item == inventory->items[i]) {
               inventory->items[i] = NULL;
               wclear(WINDOW_INVENTORY);
               break;
           }
    )
    return IE_OK;
}

static inline void add_menu_mark(WINDOW *win, Inventory *inventory, int current)
{
    if (current == inventory->selected) {
        styled(win, COLOR_PAIR(COLOR_PAIR_RED_F),
               waddstr(win, "* ");
        );
    } else {
        waddstr(win, "* ");
    }
}

void inventory_display(Inventory *inventory)
{
    Item *item;
    WINDOW *win = WINDOW_INVENTORY;

    for (int i = 0; i < inventory->size; i++) {
        wmove(win, i + PADDING, PADDING);
        add_menu_mark(win, inventory, i);

        if ((item = inventory->items[i])) {
            styled(win, item->style,
                   wprintw(win, "%lc", item->chr);
            );
            wprintw(win, " | %d | %s", abs(item->value), item->name);
        }
    }
    refresh_boxed(win);
}

void inventory_select(Inventory *inventory, Direction direction)
{
    uint16_t selected = inventory->selected;
    direction == NORTH ? selected-- : selected++;

    if (selected < inventory->size) {
        inventory->selected = selected;
    }
}

void inventory_use_selected(Player *player)
{
    Inventory *inventory = player->inventory;
    Item *selected = inventory->items[inventory->selected];

    if (!selected) {
        return;
    }

    if (CONSUMABLE == selected->type) {
        if (IE_CONSUMED == item_consume(selected, player)) {
            inventory_remove(inventory, selected);
            item_free(selected);
        }
    }
}
