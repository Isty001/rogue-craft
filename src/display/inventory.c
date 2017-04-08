#include <panel.h>
#include "../player/inventory.h"


#define SHORTCUT_LENGTH 13


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

static void add_description(Grid *grid, Item *item)
{
    Point from = point_new(
        GRID_PADDING,
        grid->size.width + GRID_PADDING * NCURSES_WIDTH_MULTIPLIER
    );

    mvwprintw(grid->window, from.y, from.x, "%s: %d", item->name, item->value);
}

static void add_inventory_to_grid(Inventory *inventory, Grid *grid)
{
    int i = 0;
    Item *item;
    List *items = inventory->items;

    grid_foreach(grid, (GridForeach) function(void, (Point point) {
        item = items->get(items, i++);
        if (item) {
            styled(grid->window, item->style,
                mvwprintw(grid->window, point.y, point.x, "%lc", item->chr);
            );

            if (point_eq(point, grid->selected)) {
                add_description(grid, item);
            }
        }
    }));
}

EventError inventory_display(InputEvent *event)
{
    Inventory *inventory = event->player->inventory;

    if (KEY_INVENTORY != event->input) {
        return EE_CONTINUE;
    }

    Grid grid;
    uint16_t side = ceil(sqrt(inventory->max_size));
    grid_new(&grid, NULL, size_new(side, side), point_new(0, 0));

    Size win_size = size_inc(grid.size, GRID_BOX_CHAR_ADJUST, GRID_BOX_CHAR_ADJUST + 10 * NCURSES_WIDTH_MULTIPLIER);
    grid.window = ncurses_newwin_adjust(win_size, WINDOW_MAIN);
    box(grid.window, 0, 0);

    add_inventory_to_grid(inventory, &grid);
    grid_display(&grid);
    panel_push_new(grid.window);

    return EE_BREAK;
}
