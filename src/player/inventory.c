#include "inventory.h"


#define SHORTCUT_LENGTH 13


Inventory *inventory_new(uint16_t max_size)
{
    Inventory *inventory = allocate(sizeof(Inventory) + (max_size * sizeof(Item *)));
    inventory->max_size = max_size;
    inventory->selected = 0;
    inventory->items = list_new();
    repeat(INVENTORY_SHORTCUT_NUM, inventory->on_shortcut[i] = NULL)

    return inventory;
}

void inventory_free(Inventory *inventory)
{
    List *items = inventory->items;

    items->free(items);
    release(inventory);
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

static void display_shortcut(Inventory *inventory, WINDOW *win, uint16_t i)
{
    List *items = inventory->items;
    Item *item = items->get(items, i);

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

    if (selected <= INVENTORY_SHORTCUT_NUM) {
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
