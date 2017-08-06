#include <malloc.h>
#include <string.h>
#include "util/memory.h"
#include "level.h"
#include "cellular.h"


static bool is_edge(Size size, int16_t neighbour_y, int16_t neighbour_x)
{
    return
        neighbour_x < 0 || neighbour_x >= size.width
        ||
        neighbour_y < 0 || neighbour_y >= size.height;
}

static uint16_t count_surrounding_walls(Cell ***cells, Size size, Point position)
{
    uint16_t count = 0;
    Size neighbourhood_size = {2, 2};
    int16_t neighbour_y, neighbour_x;

    iterate_matrix(
        -1, neighbourhood_size,

        if (y == 0 && x == 0) continue;

            neighbour_y = y + position.y;
            neighbour_x = x + position.x;

            if (is_edge(size, neighbour_y, neighbour_x)) {
                count++;
            } else if (cells[neighbour_y][neighbour_x]->type == SOLID) {
                count++;
            }
    );

    return count;
}

static void evolve(Cell ***tmp, Cell ***cells, Size size, CellFactory factory)
{
    uint16_t neighbours;

    iterate_matrix(
        0, size,

        neighbours = count_surrounding_walls(cells, size, point_new(y, x));
        tmp[y][x] = factory(cells[y][x], neighbours);
    )
}

static void fill_level_cells(Level *level)
{
    probability_pick(&level->cfg->cells.solid);

    iterate_matrix(
        0, level->size,
        level->cells[y][x] = rand_bool(0.54)
                             ? level_registry_rand(level, solid)
                             : level_registry_rand(level, hollow)
    );
}

void cellular_generate_level(Level *level)
{
    LevelCells config = level->cfg->cells;
    Cell *new;

    fill_level_cells(level);

    Cell *factory(Cell *current, uint16_t neighbours)
    {
        if (current->type == SOLID) {
            if (neighbours < 4) {
                new = probability_pick(&config.solid);
            } else {
                new = probability_pick(&config.hollow);
            }
        } else {
            if (neighbours > 4) {
                new = probability_pick(&config.hollow);
            } else {
                new = probability_pick(&config.solid);
            }
        }
        return new;
    };

    cellular_generate(level->cells, level->size, factory);
}

void cellular_generate(Cell ***cells, Size size, CellFactory factory)
{
    Cell ***tmp = level_allocate_cells(size);

    repeat(5,
       evolve(tmp, cells, size, factory);

       for (int y = 0; y < size.height; y++) {
           memcpy(cells[y], tmp[y], size.width * sizeof(Cell *));
       }
    )
    mem_dealloc(tmp);
}
