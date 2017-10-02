#include <malloc.h>
#include <string.h>
#include "memory/memory.h"
#include "level.h"
#include "cellular.h"


static bool is_edge(Level *level, int16_t neighbour_y, int16_t neighbour_x)
{
    Size size = level->size;

    return
        neighbour_x < 0 || neighbour_x >= size.width
        ||
        neighbour_y < 0 || neighbour_y >= size.height;
}

static int count_surrounding_walls(Level *level, Point position)
{
    int count = 0;
    Size neighbourhood_size = {2, 2};
    int16_t neighbour_y, neighbour_x;

    iterate_matrix(
        -1, neighbourhood_size,

        if (y == 0 && x == 0) continue;

        neighbour_y = y + position.y;
        neighbour_x = x + position.x;

        if (is_edge(level, neighbour_y, neighbour_x)) {
            count++;
        } else if (level->cells[neighbour_y][neighbour_x]->type == CELL_SOLID) {
            count++;
        }
    );

    return count;
}

static void evolve(Level *level, Cell ***tmp)
{
    int neighbours;
    Cell *cell, *new;
    LevelCells *cells = &level->cfg->cells;

    iterate_matrix(
        0, level->size,

        neighbours = count_surrounding_walls(level, point_new(y, x));
        cell = level->cells[y][x];

        if (cell->type == CELL_SOLID) {
            if (neighbours < 4) {
                new = probability_pick(&cells->solid);
            } else {
                new = probability_pick(&cells->hollow);
            }
        } else {
            if (neighbours > 4) {
                new = probability_pick(&cells->hollow);
            } else {
                new = probability_pick(&cells->solid);
            }
        }
        tmp[y][x] = new;
    )
}

static void fill_cells(Level *level)
{
    probability_pick(&level->cfg->cells.solid);

    iterate_matrix(
        0, level->size,
        level->cells[y][x] = rand_bool(0.54)
                             ? level_registry_rand(level, solid)
                             : level_registry_rand(level, hollow)
    );
}

void cellular_generate(Level *level)
{
    Size size = level->size;
    Cell ***tmp = level_allocate_cells(level->size);

    fill_cells(level);

    repeat(5,
       evolve(level, tmp);

       for (int y = 0; y < size.height; y++) {
           memcpy(level->cells[y], tmp[y], size.width * sizeof(Cell *));
       }
    )
    mem_dealloc(tmp);
}
