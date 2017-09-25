#include "ui/ncurses.h"
#include "level/cell/cell.h"
#include "sfx.h"


void sfx_play_hit(Cell *cell)
{
    sfx_play("hit", material_to_str(cell->material));
}

void sfx_play_break(Cell *cell)
{
    sfx_play("break", material_to_str(cell->material));
}

void sfx_play_step(Cell *cell)
{
    if (rand_bool(0.8)) {
        sfx_play_rand("step", material_to_str(cell->material));
    }
}
