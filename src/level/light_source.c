#include "../player/item.h"
#include "../player/inventory.h"


static void add_lighting(Player *player, Point point, LightSource *source)
{
    source->lighting = lighting_new(
        player->level, point, source->radius, source->style
    );
}

static void place_on_level(InteractionEvent *event, Item *item)
{
    Cell ***cells = event->player->level->cells;
    Point point = event->point;

    item->occupied_cell = event->cell;
    cells[point.y][point.x] = cell_with_item(item);
}

static bool is_supported(InteractionEvent *event, Item *item)
{
    return item && LIGHT_SOURCE == item->type && 1 == event->player_distance;
}

EventError item_light_source_place(InteractionEvent *event)
{
    Player *player = event->player;
    Inventory *inventory = player->inventory;
    Item *item = inventory_selected(inventory);

    if (!is_supported(event, item)) {
        return EE_CONTINUE;
    }
    inventory_remove(inventory, item);
    add_lighting(player, event->point, &item->light_source);
    place_on_level(event, item);

    return EE_BREAK;
}
