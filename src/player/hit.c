#include "player.h"
#include "item.h"
#include "inventory.h"


typedef struct {
    double damage;
    uint16_t allowed_range;
} Hit;


static double tool_damage(Tool *tool, Cell *cell)
{
    double *materials = tool->damage.materials;

    if (materials[cell->material]) {
        return materials[cell->material];
    }
    DefaultDamage defaults = tool->damage.defaults;


    return SOLID == cell->type ? defaults.solid : defaults.creature;
}

static double calculate_strength(Player *player)
{
    Attribute *attr = player->attributes;
    uint16_t positive = attr[STAMINA].current;
    uint16_t negative = attr[HUNGER].current + attr[THIRST].current;

    if (0 == negative) {
        return positive;
    }

    return positive / (negative / 100.0);
}

static Hit calculate_hit(Player *player, Item *item, Cell *target)
{
    double damage;
    uint16_t range = 1;
    double strength = calculate_strength(player);

    if (item && TOOL == item->type) {
        Tool *tool = &item->tool;

        damage = (strength / 100) * tool_damage(tool, target);
        range = tool->range;
    } else {
        damage = strength / 20;
    }

    return (Hit) {
        .damage = damage,
        .allowed_range = range
    };
}

static PlayerError apply_hit(Hit hit, Player *player, Cell *target, Point point)
{
    if (target->in_registry) {
        target = level_replace_cell_with_new(player->level, point);
    }

    target->state -= hit.damage;

    if (target->state <= 0) {
        level_set_hollow(player->level, point);
    }
    event("Dealt %d damage", (int) round(hit.damage));

    return PE_DEALT_DAMAGE;
}

PlayerError player_hit(Player *player, Point at)
{
    Cell *target = player->level->cells[at.y][at.x];

    if (!cell_damageable(target)) {
        return PE_INVALID_CELL;
    }

    Inventory *inventory = player->inventory;
    Item *item = inventory->items[inventory->selected];
    Hit hit = calculate_hit(player, item, target);

    if (hit.allowed_range < point_distance(player->position.current, at)) {
        return PE_OUT_OF_RANGE;
    }

    return apply_hit(hit, player, target, at);
}
