#include "../ncurses/ncurses.h"
#include "../player/inventory.h"
#include "camera.h"


static void pickup_item(Player *player, Cell *cell, Point point)
{
    if (false == point_are_neighbours(player->position.current, point)) {
        return;
    }

    ItemError err = inventory_add(player->inventory, cell->data);

    switch (err) {
        case IE_OK:
            level_set_hollow(player->level, point);
            cell_free_custom(cell);
            ncurses_event("Item picked up");
            break;
        case IE_OVERFLOW:
            ncurses_event("Your Inventory is full");
            break;
        case IE_DUPLICATE:
            ncurses_event("Something went wrong. The exact same item is already in your Inventory");
            break;
        default:
            ncurses_event("Unexpected error");
    }
}

void level_interact(Player *player, Point click)
{
    Point point = camera_to_level_point(player->camera, click);
    Cell ***cells = player->level->cells;
    Cell *cell = cells[point.y][point.x];

    if (ITEM_ == cell->type) {
        pickup_item(player, cell, point);
    }
}
