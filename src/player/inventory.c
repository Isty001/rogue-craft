#include "inventory.h"


#define SHORTCUT_LENGTH 13


Inventory *inventory_new(uint16_t size)
{
    Inventory *inventory = allocate(sizeof(Inventory) + (size * sizeof(Item *)));
    inventory->size = size;
    inventory->selected = 0;
    repeat(inventory->size, inventory->items[i] = NULL)
    repeat(INVENTORY_SHORTCUT_NUM, inventory->on_shortcut[i] = NULL)

    return inventory;
}

void inventory_free(Inventory *inventory)
{
    release(inventory);
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
        ncurses_event("Something went wrong. The exact same items is already in your Inventory");

        return IE_DUPLICATE;
    }

    repeat(inventory->size,
           if (NULL == inventory->items[i]) {
               inventory->items[i] = item;
               ncurses_event("Picked up %s", item->name);

               return IE_OK;
           }
    )
    ncurses_event("Your Inventory is full");

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
               wclear(WINDOW_INVENTORY_SHORTCUT);
               break;
           }
    )
    return IE_OK;
}

static void display_shortcut(Inventory *inventory, WINDOW *win, uint16_t i)
{
    Item *item = inventory->items[i];

    styled_if(win, INVENTORY_SELECTED_STYLE, i == inventory->selected,
              wprintw(win, "| %d - ", i + 1);
    )

    if (item) {
        styled(win, item->style,
               wprintw(win, "%lc", item->chr);
        );
        wprintw(win, " %d ", item->value);
    }
}

void inventory_shortcut_display(Inventory *inventory)
{
    WINDOW *win = WINDOW_INVENTORY_SHORTCUT;

    wclear(win);
    wmove(win, 1, 1);

    int displayed = min(getmaxx(win) / SHORTCUT_LENGTH, INVENTORY_SHORTCUT_NUM);
    int length = getmaxx(win) / displayed;

    for (uint16_t i = 0; i < displayed; i++) {
        display_shortcut(inventory, win, i);
        wmove(win, 1, (i + 1) * length);
    }

    refresh_boxed(win);
}

EventError inventory_shortcut_select(InputEvent *event)
{
    int input = event->input;

    if (input < INVENTORY_SHORTCUT_FIRST || input >= INVENTORY_SHORTCUT_FIRST + INVENTORY_SHORTCUT_NUM) {
        return EE_CONTINUE;
    }

    Inventory *inventory = event->player->inventory;
    uint16_t selected = input % INVENTORY_SHORTCUT_FIRST;

    if (selected < inventory->size) {
        inventory->selected = selected;
    }

    return EE_OK;
}

void inventory_use_selected(Player *player)
{
    Inventory *inventory = player->inventory;
    Item *selected = inventory_selected(inventory);

    if (NULL == selected) {
        return;
    }
    if (CONSUMABLE == selected->type) {
        if (IE_CONSUMED == item_consume(selected, player)) {
            inventory_remove(inventory, selected);
            item_free(selected);
        }
    }
}
