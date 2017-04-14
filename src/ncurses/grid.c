#include <ncurses.h>
#include <stdint.h>
#include "../util.h"
#include "../level/point.h"
#include "../level/level.h"


#define INC_Y GRID_CELL_SIZE
#define INC_X GRID_CELL_SIZE + NCURSES_WIDTH_MULTIPLIER


void grid_new(Grid *grid, WINDOW *window, Size size, Point left_upper)
{
    grid->window = window;
    grid->size = size;
    grid->size.height *= INC_Y;
    grid->size.width *= INC_X;
    grid->left_upper = left_upper;
    grid->selected = point_new(GRID_PADDING, GRID_PADDING);
}

static Point bottom_right_of(Grid *grid)
{
    return point_new(
        grid->left_upper.y + grid->size.height,
        grid->left_upper.x + grid->size.width
    );
}

void grid_display(Grid *grid)
{
    WINDOW *window = grid->window;
    Point from = grid->left_upper;
    Size size = grid->size;
    Point until = bottom_right_of(grid);

    Point selected = grid->selected;

    for (int y = from.y; y <= until.y; y += INC_Y) {
        mvwhline(window, y, from.x, 0, size.width);
        if (y + GRID_PADDING == selected.y) {
            styled(window, COLOR_PAIR(COLOR_RED_F_B),
                mvwaddch(window, y + 1, from.x + 3, ' ');
            );
        }
    }

    for (int x = from.x; x <= until.x; x += INC_X) {
        mvwvline(window, from.y + GRID_BOX_CHAR_ADJUST, x, 0, size.height - GRID_BOX_CHAR_ADJUST);
    }

    mvwaddch(window, from.y, from.x, ACS_ULCORNER);
    mvwaddch(window, from.y + size.height, from.x, ACS_LLCORNER);
    mvwaddch(window, from.y, from.x + size.width, ACS_URCORNER);
    mvwaddch(window, from.y + size.height, from.x + size.width, ACS_LRCORNER);
}

void grid_foreach(Grid *grid, GridForeach visit)
{
    Point from = grid->left_upper;
    Point until = bottom_right_of(grid);

    for (int y = from.y; y < until.y; y += INC_Y) {
        for (int x = from.x; x < until.x; x += INC_X) {
            visit(point_new(y + GRID_PADDING, x + GRID_PADDING));
        }
    }
}
