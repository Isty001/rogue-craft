#ifndef ROGUE_CRAFT_GRID_H
#define ROGUE_CRAFT_GRID_H


#include <ncursesw/ncurses.h>
#include "level/point.h"


#define GRID_CELL_SIZE 2
#define GRID_BOX_CHAR_ADJUST 1
#define GRID_PADDING 1


typedef void (*GridForeach)(Point display, bool selected);

typedef struct {
    WINDOW *window;
    Size size;
    Point left_upper;
    Point selected;
    Bounds bounds;
} Grid;


Grid *grid_new(WINDOW *window, Size size, Point left_upper);

void grid_free(Grid *grid);

void grid_display(Grid *grid);

void grid_move_selected(Grid *grid, Direction direction);

void grid_foreach(Grid *grid, GridForeach visit);


#endif
