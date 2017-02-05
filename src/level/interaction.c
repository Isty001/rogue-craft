#include "../ncurses/ncurses.h"
#include "../player/inventory.h"
#include "camera.h"


static void pickup_item(Player *player, Cell *cell, Point point)
{
    if (1 != point_distance(player->position.current, point)) {
        return;
    }

    ItemError err = inventory_add(player->inventory, cell->item);

    switch (err) {
        case IE_OK:
            level_set_hollow(player->level, point);
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
    Point on_level = camera_to_level_point(player->camera, click);
    Cell ***cells = player->level->cells;
    Cell *cell = cells[on_level.y][on_level.x];

    switch(cell->type){
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
