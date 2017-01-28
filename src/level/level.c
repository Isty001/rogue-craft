#include "level.h"
#include "../ncurses/ncurses.h"
#include "camera.h"


static void allocate_rows(Level *level)
{
    Size size = level->size;
        
    Cell ***cells = alloc(sizeof(Cell[size.height][size.width]));
    Cell **row = (Cell **) cells + size.height;

    for (int i = 0; i < size.height; i++) {
        cells[i] = row + i * size.height;
    }

    level->cells = cells;
}

static void initialize_cells(Level *level)
{
    iterate_matrix(
        0, level->size,
        level->cells[y][x] = rand_true(0.54) ? &cell_registry_rand(level, solid) : &cell_registry_rand(level, hollow);
    );
}

Level *level_new(Size size, LevelConfig cfg)
{
    Level *level = alloc(sizeof(Level));
    level->size = size;

    level_add_bounds(level);
    level->registry.hollow = cell_registry_new(*cfg.cell.hollow);
    level->registry.solid = cell_registry_new(*cfg.cell.solid);

    allocate_rows(level);
    initialize_cells(level);

    if (CAVE == cfg.type) {
        level_generate_cave(level);
    }

    return level;
}

void level_free(Level *level)
{
    Cell *cell;
    Size size = level->size;

    iterate_matrix(
        0, size,

        if (!(cell = level->cells[y][x])->is_prototype) {
            free(cell);
        }

        if (x == size.width - 1) {
            free(level->cells[y]);
        }
    )

    free(level->registry.hollow.cells);
    free(level->registry.solid.cells);
    free(level->cells);
    free(level);
}

void level_set_hollow(Level *level, Point at)
{
    level->cells[at.y][at.x] = &cell_registry_rand(level, hollow);
}

static Point displayed_bounds(Camera *camera)
{
    return point_new(
        camera->position.y + camera->size.height,
        camera->position.x + camera->size.width
    );
}

void level_display(Player *player)
{
    Cell *cell;
    Point win_pos = point_new(0, 0);
    Camera *camera = player->camera;
    Point until = displayed_bounds(camera);
    Cell ***cells = player->level->cells;

    wclear(WINDOW_MAIN);

    for (int y = camera->position.y; y <= until.y; y++) {
        for (int x = camera->position.x; x <= until.x; x++) {
            cell = cells[y][x];
            if (player_can_see(player, y, x) || true) {
                wattron(WINDOW_MAIN, COLOR_PAIR(cell->color));
                mvwaddch(WINDOW_MAIN, win_pos.y, win_pos.x, cell->chr);
            }
            win_pos.x++;
        }
        win_pos.y++;
        win_pos.x = 0;
    }

    wrefresh(WINDOW_MAIN);
}

