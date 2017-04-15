#include "../level/level.h"
#include "../level/camera.h"


static Point displayed_bounds(Camera *camera)
{
    return point_new(
        camera->position.y + camera->size.height,
        camera->position.x + camera->size.width
    );
}

static void display_visible(Player *player, Cell *cell, Point point, Point win_pos)
{
    if (player_can_see(player, cell, point)) {
        styled(WINDOW_MAIN, cell->style,
               mvwprintw(WINDOW_MAIN, win_pos.y, win_pos.x, "%lc", cell->chr)
        );
    }
}

void level_display(Player *player)
{
    Point win_pos = point_new(0, 0);
    Camera *camera = player->camera;
    Point until = displayed_bounds(camera);
    Cell ***cells = player->level->cells;

    wclear(WINDOW_MAIN);

    for (uint16_t y = camera->position.y; y <= until.y; y++) {
        for (uint16_t x = camera->position.x; x <= until.x; x++) {
            display_visible(player, cells[y][x], point_new(y, x), win_pos);
            win_pos.x++;
        }
        win_pos.y++;
        win_pos.x = 0;
    }

    wrefresh(WINDOW_MAIN);
}
