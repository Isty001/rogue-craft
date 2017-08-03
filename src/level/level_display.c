#include "level/level.h"
#include "level/camera.h"
#include "lighting/lighting.h"


static Point displayed_bounds(Camera *camera)
{
    return point_new(
        camera->left_upper.y - CAMERA_BORDER + camera->size.height,
        camera->left_upper.x - CAMERA_BORDER + camera->size.width
    );
}

static inline bool in_sight(Player *player, Point point)
{
    return sight_has(player->sight, point) || point_eq(point, player->position.current);
}

static inline Lighting *find_lighting(List *lightings, Point point)
{
    return lightings->find(lightings, (Predicate) function(bool, (Lighting *lighting) {
        return sight_has(lighting->sight, point);
    }));
}

static void display_visible(Player *player, Cell *cell, Point point, Point win_pos)
{
    Lighting *lighting = NULL;

    if (in_sight(player, point)) {
        styled(WINDOW_MAIN, cell->style,
               mvwprintw(WINDOW_MAIN, win_pos.y, win_pos.x, "%lc", cell->chr)
        );
    } else if ((lighting = find_lighting(player->level->lightings, point))) {
        if (CELL_SOLID == cell->type){
            styled(WINDOW_MAIN, lighting->style,
                   mvwprintw(WINDOW_MAIN, win_pos.y, win_pos.x, "%lc", cell->chr)
            );
        } else {
            styled(WINDOW_MAIN, cell->style,
                   mvwprintw(WINDOW_MAIN, win_pos.y, win_pos.x, "%lc", cell->chr)
            );
        }
    } else {
        mvwaddch(WINDOW_MAIN, win_pos.y, win_pos.x, ' ');
    }
}

void level_display(Player *player)
{
    Point win_pos = point_new(0, 0);
    Camera *camera = player->camera;
    Point until = displayed_bounds(camera);
    Cell ***cells = player->level->cells;

    for (uint16_t y = camera->left_upper.y; y < until.y; y++) {
        for (uint16_t x = camera->left_upper.x; x < until.x; x++) {
            display_visible(player, cells[y][x], point_new(y, x), win_pos);
            win_pos.x++;
        }
        win_pos.y++;
        win_pos.x = 0;
    }

    wrefresh(WINDOW_MAIN);
}
