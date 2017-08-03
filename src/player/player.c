#include <time.h>
#include "util/memory.h"
#include "player.h"
#include "inventory/inventory.h"


static void init_modifiers(Player *player)
{
    Modifiers *modifiers = &player->state.modifiers;

    modifiers->traveled = 0;
    modifiers->dealt_damage = 0;
    modifiers->timestamp.fatigue = time(NULL);
    modifiers->timestamp.fatigue_damage = time(NULL);
}

static void add_default_attributes(Player *player)
{
    Attribute *map = player->state.attributes;

    map[HEALTH] = (Attribute) {
        .current = 100, .max = 100,
        .name = "Health", .style  = COLOR_PAIR(COLOR_RED_F)
    };

    map[STAMINA] = (Attribute) {
        .current = 100, .max = 100,
        .name = "Stamina", .style = COLOR_PAIR(COLOR_GREEN_F)
    };

    map[HUNGER] = (Attribute) {
        .current = 0, .max = 100,
        .name = "Hunger", .style = COLOR_PAIR(COLOR_YELLOW_F)
    };

    map[THIRST] = (Attribute) {
        .current = 0, .max = 100,
        .name = "Thirst", .style = COLOR_PAIR(COLOR_BLUE_F)
    };
}

static void find_starting_point(Player *player)
{
    Level *level = player->level;
    Point point = level_rand_hollow(level);

    player->cell.occupied = level->cells[point.y][point.x];
    player->position.previous = point;
    player->position.current = point;
}

Player *player_new(Level *level, Camera *camera)
{
    Player *player = mem_alloc(sizeof(Player));
    player->level = level;
    player->camera = camera;

    add_default_attributes(player);
    init_modifiers(player);

    player->cell.prototype.type = CELL_PLAYER;
    player->cell.prototype.style = COLOR_PAIR(COLOR_RED_F);
    player->cell.prototype.chr = PLAYER_CHAR;

    find_starting_point(player);

    player->inventory = inventory_new(PLAYER_DEFAULT_INVENTORY_SIZE);
    player->sight = sight_new(level, player->position.current, PLAYER_DEFAULT_EYESIGHT);

    return player;
}

void player_free(Player *player)
{
    inventory_free(player->inventory);
    sight_free(player->sight);
    mem_dealloc(player);
}

void player_sight_update(Player *player)
{
    sight_update(player->sight, player->position.current, PLAYER_DEFAULT_EYESIGHT);
}
