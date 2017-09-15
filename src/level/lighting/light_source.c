#include <level/level.h>
#include "item/item.h"
#include "inventory/inventory.h"


static void add_lighting(Player *player, Point point, LightSource *source)
{
    source->lighting = lighting_new(
        player->level, point, source->radius, source->style
    );
}

static void place_on_level(LevelInteractionEvent *event, Item *item)
{
    Cell ***cells = event->player->level->cells;
    Point point = event->point;

    item->occupied_cell = event->cell;
    cells[point.y][point.x] = cell_from_item(item);
}

static bool is_supported(LevelInteractionEvent *event, Item *item)
{
    return
        1 == event->player_distance
        && item && LIGHT_SOURCE == item->type
        && HOLLOW == event->cell->type;
}

EventStatus item_light_source_place(LevelInteractionEvent *event)
{
    Player *player = event->player;
    Inventory *inventory = player->inventory;
    Item *item = inventory_selected(inventory);

    if (!is_supported(event, item)) {
        return ES_CONTINUE;
    }
    inventory_remove(inventory, item);
    place_on_level(event, item);
    add_lighting(player, event->point, &item->light_source);

    return ES_BREAK;
}
