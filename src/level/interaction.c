#include "../ncurses/ncurses.h"
#include "../player/inventory.h"
#include "camera.h"


static void pickup_item(Player *player, Cell *cell, Point point)
{
    if (1 != point_distance(player->position.current, point)) {
        return;
    }

    if (IE_OK == inventory_add(player->inventory, cell->item)) {
        level_set_hollow(player->level, point);
    }
}

void level_interact(Player *player, Point click)
{
    Point on_level = camera_to_level_point(player->camera, click);
    Cell ***cells = player->level->cells;
    Cell *cell = cells[on_level.y][on_level.x];

    switch (cell->type) {
        case ITEM_:
            pickup_item(player, cell, on_level);
            break;
        case SOLID:
            player_hit(player, on_level);
            break;
        default:
            return;
    }
}
