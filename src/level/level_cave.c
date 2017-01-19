#include <malloc.h>
#include <string.h>
#include "level.h"
#include "point.h"


static bool is_edge(Level *level, Point neighbour)
{
    Size size = level->size;

    return
        neighbour.x < 0 || neighbour.x >= size.width
        ||
        neighbour.y < 0 || neighbour.y >= size.height;
}

static int count_surrounding_walls(Level *level, Point position)
{
    int count = 0;
    Size neighbourhood_size = {2, 2};
    Point neighbour;

    iterate_matrix(
        -1, neighbourhood_size,

        if (y == 0 && x == 0) continue;

        neighbour = point_new(
            y + position.y,
            x + position.x
        );

        if (is_edge(level, neighbour)) {
            count++;
        } else if (level->cells[neighbour.y][neighbour.x]->type == SOLID) {
            count++;
        }
    );

    return count;
}

static void evolve(Level *level, Cell *(*tmp)[level->size.height][level->size.width])
{
    int neighbours;
    Cell *cell, *new;

    iterate_matrix(
        0, level->size,

        neighbours = count_surrounding_walls(level, point_new(y, x));
        cell = level->cells[y][x];

        if (cell->type == SOLID) {
            if (neighbours < 4) {
                new = &cell_registry_rand(level, solid);
            } else {
                new = &cell_registry_rand(level, hollow);
            }
        } else {
            if (neighbours > 4) {
                new = &cell_registry_rand(level, hollow);
            } else {
                new = &cell_registry_rand(level, solid);
            }
        }
        (*tmp)[y][x] = new;
    )
}

void level_generate_cave(Level *level)
{
    Size size = level->size;
    Cell *tmp[level->size.height][level->size.width];

    repeat(5,
       evolve(level, &tmp);

       for (int y = 0; y < size.height; y++) {
           memcpy(level->cells[y], tmp[y], size.width * sizeof(Cell *));
       }
    )
}
