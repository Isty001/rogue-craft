#include "../ncurses/ncurses.h"
#include "../../config/config.h"
#include "camera.h"


static void allocate_rows(Level *level)
{
    Size size = level->size;

    Cell ***cells = alloc(sizeof(Cell[size.height][size.width]));
    Cell **from = (Cell **) cells + size.height;

    for (int i = 0; i < size.height; i++) {
        cells[i] = from + i * size.height;
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

static void add_items(Level *level)
{
    uint16_t remaining = level->item_count;
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

Level *level_new(Size size, LevelConfig *cfg)
{
    Level *level = alloc(sizeof(Level));
    level->size = size;
    level->item_count = LEVEL_ITEM_COUNT(level);

    level_add_bounds(level);
    level->registry.hollow = cell_registry_new(cfg->cell.hollow);
    level->registry.solid = cell_registry_new(cfg->cell.solid);

    allocate_rows(level);
    initialize_cells(level);

    if (CAVE == cfg->type) {
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
    level->cells[at.y][at.x] = &cell_registry_rand(level, hollow);
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
    free(level->registry.hollow.cells);
    free(level->registry.solid.cells);
    free(level->cells);
    free(level);
}
