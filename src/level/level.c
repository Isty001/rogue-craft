#include "../ncurses/ncurses.h"
#include "../../config/config.h"
#include "camera.h"


static void allocate_rows(Level *level)
{
    Size size = level->size;

    Cell ***cells = allocate(sizeof(Cell[size.height][size.width]));
    Cell **from = (Cell **) cells + size.height;

    for (int i = 0; i < size.height; i++) {
        cells[i] = from + i * size.height;
    }

    level->cells = cells;
}

static void initialize_cells(Level *level)
{
    probability_pick(&level->cfg->cells.solid);

    iterate_matrix(
        0, level->size,
        level->cells[y][x] = rand_bool(0.54)
                             ? probability_pick(&level->cfg->cells.solid)
                             : probability_pick(&level->cfg->cells.hollow)
    );
}

static void add_items(Level *level)
{
    uint16_t remaining = LEVEL_ITEM_COUNT(level);
    Point point;
    Cell *cell;

    do {
        point = level_rand_point(level);
        cell = level->cells[point.y][point.x];

        if (HOLLOW == cell->type) {
            level->cells[point.y][point.x] = cell_with_random_item();
            remaining--;
        };
    } while (remaining);
}

Level *level_new(Size size)
{
    Level *level = allocate(sizeof(Level));
    level->size = size;
    level->cfg = probability_pick(&LEVEL_PROBABILITY);

    level_add_bounds(level);

    allocate_rows(level);
    initialize_cells(level);

    if (CELLULAR == level->cfg->type) {
        level_generate_cave(level);
    }

    add_items(level);

    return level;
}

bool level_in_bounds(Level *level, Point point)
{
    return
        in_range(point.x, level->bounds.x)
        &&
        in_range(point.y, level->bounds.y);
}

Point level_rand_hollow(Level *level)
{
    Point point;
    Cell *cell;

    do {
        point = level_rand_point(level);

        cell = level->cells[point.y][point.x];
    } while (HOLLOW != cell->type);

    return point;
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

    for (uint16_t y = camera->position.y; y <= until.y; y++) {
        for (uint16_t  x = camera->position.x; x <= until.x; x++) {
            cell = cells[y][x];

            if (player_can_see(player, point_new(y, x))) {
                styled(WINDOW_MAIN, cell->style,
                       mvwprintw(WINDOW_MAIN, win_pos.y, win_pos.x, "%lc", cell->chr)
                );
            }
            win_pos.x++;
        }
        win_pos.y++;
        win_pos.x = 0;
    }

    wrefresh(WINDOW_MAIN);
}

void level_set_hollow(Level *level, Point at)
{
    cell_free_custom(level->cells[at.y][at.x]);
    level->cells[at.y][at.x] = probability_pick(&level->cfg->cells.hollow);
}

Cell *level_replace_cell_with_new(Level *level, Point at)
{
    Cell ***cells = level->cells;
    Cell *cell = cells[at.y][at.x];
    cell_free_custom(cell);
    Cell *new = cell_clone(cell);

    cells[at.y][at.x] = new;

    return new;
}

void level_free(Level *level)
{
    release(level->cells);
    release(level);
}
