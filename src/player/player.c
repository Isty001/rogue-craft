#include <memory.h>
#include "player.h"
#include "../../data/config.h"
#include "inventory.h"


#define attribute(player, attr_name, def_curr, def_limit, type, disp_name, attr_color) \
    player->attr.attr_name.limit = def_limit;                       \
    player->attr.attr_name.current = def_curr;                      \
    strcpy(player->attr.attr_name.name, disp_name);                 \
    player->attr.attr_name.color = attr_color;                      \
    player->attr.type_map[type] = &player->attr.attr_name;          \


static void calculate_starting_point(Player *player)
{
    Level *level = player->level;
    Cell *cell;
    Point point;

    do {
        point = level_rand_point(level);

        cell = level->cells[point.y][point.x];
    } while (HOLLOW != cell->type);

    player->cell.previous = cell;
    player->position.previous = point;
    player->position.current = point;
}

static void add_attributes(Player *player)
{
    attribute(player, hp, PLAYER_DEFAULT_HP, PLAYER_DEFAULT_HP, HEALTH, "Health", COLOR_HEALTH);
    attribute(player, stamina, PLAYER_DEFAULT_STAMINA, PLAYER_DEFAULT_STAMINA, STAMINA, "Stamina", COLOR_STAMINA);
    attribute(player, hunger, PLAYER_DEFAULT_HUNGER, PLAYER_LIMIT_HUNGER, HUNGER, "Hunger", COLOR_FOOD);
    attribute(player, thirst, PLAYER_DEFAULT_THIRST, PLAYER_LIMIT_THIRST, THIRST, "Thirst", COLOR_WATER);
}

Player *player_new(Level *level, Camera *camera)
{
    Player *player = alloc(sizeof(Player));
    player->level = level;
    player->cell.prototype.type = PLAYER;
    player->eyesight = PLAYER_DEFAULT_EYESIGHT;
    player->camera = camera;

    add_attributes(player);

    player->cell.prototype.style = COLOR_PAIR(COLOR_PAIR_RED_F);
    player->cell.prototype.chr = PLAYER_CHAR;

    calculate_starting_point(player);

    player->inventory = inventory_new(PLAYER_DEFAULT_INVENTORY_SIZE);

    return player;
}

void player_display_stats(Player *player)
{
    Attribute **attributes = player->attr.type_map;
    Attribute *attr;

    wclear(WINDOW_PLAYER_STATS);

    for (int i = 0; i < PLAYER_ATTR_NUM; i++) {
        attr = attributes[i];

        wattron(WINDOW_PLAYER_STATS, COLOR_PAIR(attr->color));
        mvwprintw(WINDOW_PLAYER_STATS, i + 1, 2, attr->name);
        wattroff(WINDOW_PLAYER_STATS, COLOR_PAIR(attr->color));

        wprintw(WINDOW_PLAYER_STATS, ": %d/%d", attr->current, attr->limit);
    }
    wrefreshbox(WINDOW_PLAYER_STATS);
}

void player_free(Player *player)
{
    inventory_free(player->inventory);
    free(player);
}
