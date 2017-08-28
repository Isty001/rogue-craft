#include "memory/memory.h"
#include "inventory.h"


Inventory *inventory_new(uint16_t max_size)
{
    Inventory *inventory = mem_alloc(sizeof(Inventory) + (max_size * sizeof(Item *)));
    inventory->max_size = max_size;
    inventory->selected = 0;
    inventory->grid = NULL;
    inventory->items = list_new();

    return inventory;
}

void inventory_free(Inventory *inventory)
{
    List *items = inventory->items;

    items->free(items);
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
    ncurses_event("Picked up a(n) %s", item->name);

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

EventError inventory_player_shortcut_select(InputEvent *event)
{
    int input = event->input;

    if (!inventory_is_shortcut(input)) {
        return EE_CONTINUE;
    }

    Inventory *inventory = event->player->inventory;
    uint16_t selected = input % INVENTORY_SHORTCUT_FIRST;

    if (selected <= INVENTORY_SHORTCUT_NUM) {
        inventory->selected = selected;
    }

    return EE_BREAK;
}

void inventory_use_selected(Inventory *inventory, Player *player)
{
    Item *selected = inventory_selected(inventory);

    if (selected) {
        if (CONSUMABLE == selected->type) {
            if (IE_CONSUMED == item_consume(selected, player)) {
                inventory_remove(inventory, selected);
                item_free(selected);
            }
        }
    }
}

EventError inventory_player_use_selected(InputEvent *event)
{
    if (KEY_USE != event->input) {
        return EE_CONTINUE;
    }

    Inventory *inventory = event->player->inventory;

    inventory_use_selected(inventory, event->player);

    return EE_BREAK;
}
