#include <player/sight.h>
#include <player/player.h>
#include "lighting/lighting.h"


static inline Style style_of(Lighting *lighting, Cell *cell)
{
    if (lighting && CELL_SOLID == cell->type) {
        return lighting->style;
    }

    return cell->style;
}

static inline void display_sight(Sight *sight, Player *player, Lighting *lighting)
{
    Camera *camera = &player->camera;

    if (!camera_has_sigh(camera, sight)){
        return;
    }

    Cell *cell, ***cells = player->level->cells;
    Point on_level, on_camera;

    repeat(sight->point_count,
           on_level = sight->points[i];
           // This might return a point that is actually out of the camera, but mvwprintw just won't bother with it
           on_camera = camera_level_to_camera_point(camera, on_level);
           cell = cells[on_level.y][on_level.x];

           styled(WINDOW_MAIN, style_of(lighting, cell),
                  mvwprintw(WINDOW_MAIN, on_camera.y, on_camera.x, "%lc", cell->chr)
           );
    )
}

void level_display(Player *player)
{
    wclear(WINDOW_MAIN);

    display_sight(player->sight, player, NULL);
    List *lightings = player->level->lightings;

    lightings->foreach_l(lightings, (Foreach) function(void, (Lighting * lighting) {
        display_sight(lighting->sight, player, lighting);
    }));

    wrefresh(WINDOW_MAIN);
}
