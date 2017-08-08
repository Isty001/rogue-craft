#include <ncurses/ncurses.h>
#include "level/cell/cell.h"
#include "sfx.h"


static char *name_of(Cell *cell)
{
    switch (cell->material) {
        case STONE:
            return "stone";
        case WOOD:
            return "wood";
        case DIRT:
            return "dirt";
        case SNOW:
            return "snow";
        case SAND:
            return "sand";
        case WATER:
            return "water";
        default:
            return NULL;
    }
}

void sfx_play_hit(Cell *cell)
{
    sfx_play("hit", name_of(cell));
}

void sfx_play_break(Cell *cell)
{
    sfx_play("break", name_of(cell));
}

void sfx_play_step(Cell *cell)
{
    if (rand_bool(0.5)) {
        sfx_play_rand("step", name_of(cell));
    }
}
