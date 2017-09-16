#include <level/point.h>
#include "memory/memory.h"
#include "inventory.h"


#define MESSAGE_FULL "Your Inventory is full"


Inventory *inventory_new(uint16_t max_size)
{
    Inventory *inventory = mem_alloc(sizeof(Inventory));
    inventory->max_size = max_size;
    inventory->selected = 0;
    inventory->count = 0;
    inventory->grid = NULL;
    inventory->items = mem_alloc(max_size * sizeof(Item *));
    repeat(max_size, inventory->items[i] = NULL);

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

bool inventory_has(Inventory *inventory, Item *item)
{
    repeat(inventory->max_size,
           if (item == inventory->items[i]) {
               return true;
           }
    )

    return false;
}

static ItemError replace_item(Inventory *inventory, Item *from, Item *to)
{
    Item **items = inventory->items;

    repeat(inventory->max_size,
           if (from == items[i]) {
               items[i] = to;

               return IE_OK;
           }
    )
    return IE_OVERFLOW;
}

ItemError inventory_add(Inventory *inventory, Item *item)
{
    if (inventory_has(inventory, item)) {
        return IE_DUPLICATE;
    }

    if (inventory->max_size == inventory->count) {
        ncurses_event(MESSAGE_FULL);
        return IE_OVERFLOW;
    }
    ItemError err = replace_item(inventory, NULL, item);

    if (IE_OK == err) {
        ncurses_event("%s added to your inventory", item->name);
    } else {
        ncurses_event(MESSAGE_FULL);
    }

    return err;
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

ItemError inventory_remove(Inventory *inventory, Item *item)
{
    if (!inventory_has(inventory, item)) {
        return IE_INVALID_ARGUMENT;
    }

    replace_item(inventory, item, NULL);
    wclear(WINDOW_INVENTORY_SHORTCUT);

    return IE_OK;
}

static ItemError drop_item(Point point, Level *level, Item *item, Inventory *inventory)
{
    Cell ***cells = level->cells;
    Cell *cell = cells[point.y][point.x];

    if (HOLLOW == cell->type && level_in_bounds(level, point)) {
        cells[point.y][point.x] = cell_from_item(item);

        return inventory_remove(inventory, item);
    }

    return IE_INVALID_ARGUMENT;
}

ItemError inventory_drop_selected(Inventory *inventory, Point around, Level *level)
{
    Item *selected = inventory_selected(inventory);

    if (!selected) {
        return IE_INVALID_ARGUMENT;
    }

    Bounds bounds = level->bounds;
    Point from = point_new(max(0, around.y - 1), max(0, around.x - 1));
    Point until = point_new(min(bounds.y.to, around.y + 1), min(bounds.x.to, around.x + 1));

    for (uint16_t y = from.y; y <= until.y; y++) {
        for (uint16_t x = from.x; x <= until.x; x++) {
            if (IE_OK == drop_item(point_new(y, x), level, selected, inventory)) {
                return IE_OK;
            }

        }
    }
    ncurses_event("No more free space around you");

    return IE_OVERFLOW;
}
