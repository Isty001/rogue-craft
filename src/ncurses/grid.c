#include <ncurses.h>
#include <stdint.h>
#include "ncurses.h"
#include "grid.h"
#include "../color.h"


#define INC_Y GRID_CELL_SIZE
#define INC_X GRID_CELL_SIZE + NCURSES_WIDTH_MULTIPLIER
#define SELECTED_STYLE COLOR_PAIR(COLOR_RED_F_B)
#define NON_SELECTED_STYLE COLOR_PAIR(COLOR_NONE)


Grid *grid_new(WINDOW *window, Size size, Point left_upper)
{
    Grid *grid = allocate(sizeof(Grid));
    grid->window = window;
    grid->size = size;
    grid->size.height *= INC_Y;
    grid->size.width *= INC_X;
    grid->left_upper = left_upper;
    grid->selected = point_new(0, 0);
    grid->bounds = bounds_new(size);

    return grid;
}

void grid_free(Grid *grid)
{
    release(grid);
}

static Point bottom_right_of(Grid *grid)
{
    return point_new(
        grid->left_upper.y + grid->size.height,
        grid->left_upper.x + grid->size.width
    );
}

static void add_corners(WINDOW *window, Point from, Size size)
{
    mvwaddch(window, from.y, from.x, ACS_ULCORNER);
    mvwaddch(window, from.y + size.height, from.x, ACS_LLCORNER);
    mvwaddch(window, from.y, from.x + size.width, ACS_URCORNER);
    mvwaddch(window, from.y + size.height, from.x + size.width, ACS_LRCORNER);
}

static void mark_cell(Grid *grid, Point point, Style style)
{
    point.y *= INC_Y;
    point.y += GRID_PADDING;

    point.x *= INC_X;
    point.x += 3;

    styled(grid->window, style,
           mvwprintw(grid->window, point.y, point.x, " ")
    );
}

void grid_display(Grid *grid)
{
    WINDOW *window = grid->window;
    Point from = grid->left_upper;
    Size size = grid->size;
    Point until = bottom_right_of(grid);

    for (int y = from.y; y <= until.y; y += INC_Y) {
        mvwhline(window, y, from.x, 0, size.width);
    }

    for (int x = from.x; x <= until.x; x += INC_X) {
        mvwvline(window, from.y + GRID_BOX_CHAR_ADJUST, x, 0, size.height - GRID_BOX_CHAR_ADJUST);
    }

    add_corners(window, from, size);
}

void grid_move_selected(Grid *grid, Direction direction)
{
    Point new = point_move(grid->selected, direction, 1);

    if (bounds_has(grid->bounds, new) && !point_eq(grid->selected, new)){
        mark_cell(grid, grid->selected, NON_SELECTED_STYLE);
        mark_cell(grid, new, SELECTED_STYLE);
        grid->selected = new;
    }
}

void grid_foreach(Grid *grid, GridForeach visit)
{
    Point from = grid->left_upper;
    Point until = bottom_right_of(grid);
    Point display, position = point_new(0, 0);
    bool selected;

    for (int y = from.y; y < until.y; y += INC_Y) {
        for (int x = from.x; x < until.x; x += INC_X) {
            display =  point_new(y + GRID_PADDING, x + GRID_PADDING);
            selected = point_eq(grid->selected, position);

            visit(display, selected);
            position.x++;
        }
        position.x = 0;
        position.y++;
    }
}
