#include "ui/grid.h"
#include "ui/ncurses.h"
#include "item/item.h"
#include "inventory.h"


#define DESCRIPTION_LINE_PADDING 2 * (NCURSES_WIDTH_MULTIPLIER * GRID_PADDING)


static Point description_from(Grid *grid)
{
    return point_new(
        GRID_PADDING,
        grid->size.width + GRID_PADDING * NCURSES_WIDTH_MULTIPLIER
    );
}

static void clear_description(Grid *grid)
{
    Point from = description_from(grid);
    uint16_t desc_width = getmaxx(grid->window) - grid->size.width - DESCRIPTION_LINE_PADDING;

    mvwhline(grid->window, from.y, from.x, ' ', desc_width);
}

static void add_description(Grid *grid, Item *item, bool selected)
{
    Point from = description_from(grid);

    if (item && selected) {
        clear_description(grid);
        mvwprintw(grid->window, from.y, from.x, "%s: %d", item->name, item_value(item));
    } else if (!item && selected) {
        clear_description(grid);
    }
}

static void add_item(Grid *grid, Item *item, Point display)
{
    if (item) {
        styled(grid->window, item->style,
               mvwprintw(grid->window, display.y, display.x, "%lc", item->chr);
        );
    } else {
        mvwprintw(grid->window, display.y, display.x, " ");
    }
}

static void add_inventory_to_grid(Inventory *inventory)
{
    int i = 0;
    Item *item;
    List *items = inventory->items;
    Grid *grid = inventory->grid;

    grid_foreach(grid, (GridForeach) function(void, (Point display, bool selected) {
        item = items->get(items, i++);

        add_item(grid, item, display);
        add_description(grid, item, selected);
    }));
}

void inventory_grid_update(Inventory *inventory)
{
    Grid *grid = inventory->grid;

    add_inventory_to_grid(inventory);
    grid_display(grid);
}

static void set_grid_window(Grid *grid)
{
    Size win_size = size_inc(grid->size, GRID_BOX_CHAR_ADJUST, GRID_BOX_CHAR_ADJUST + 10 * NCURSES_WIDTH_MULTIPLIER);

    grid->window = ncurses_newwin_adjust(win_size, WINDOW_MAIN);
    box(grid->window, 0, 0);
}

static Point selected_point(uint16_t size, uint16_t selected)
{
    return point_new(
        selected / size,
        selected % size
    );
}

static Grid *create_grid(Inventory *inventory)
{
    uint16_t size = inventory_grid_size(inventory);

    Grid *grid = grid_new(NULL, size_new(size, size), point_new(0, 0));
    grid->selected = selected_point(size, inventory->selected);

    return grid;
}

EventStatus inventory_display(Inventory *inventory)
{
    if (inventory->grid) {
        return ES_CONTINUE;
    }

    inventory->grid = create_grid(inventory);

    set_grid_window(inventory->grid);
    inventory_grid_update(inventory);

    PanelInfo info = {
        .type = PANEL_INVENTORY, .inventory = inventory
    };
    panel_push_new(inventory->grid->window, info);

    return ES_BREAK;
}
