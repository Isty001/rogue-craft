#include <level/point.h>
#include <util/util.h>
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

 bool inventory_has(Inventory *inventory, Item *item)
{
    List *items = inventory->items;

    return items->has(items, item);
}

ItemError inventory_add(Inventory *inventory, Item *item)
{
    if (inventory_has(inventory, item)) {
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
    if (!inventory_has(inventory, item)) {
        return IE_INVALID_ARGUMENT;
    }
    List *items = inventory->items;

    items->delete(items, item);
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
    ncurses_event("No more free space around you");

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

    return IE_OVERFLOW;
}
