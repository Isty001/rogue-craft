
#include "player.h"
#include "item/item.h"
#include "inventory/inventory.h"
#include "sfx/sfx.h"


typedef struct {
    int16_t damage;
    uint16_t allowed_range;
} Hit;


static double tool_damage_multiplier(Tool *tool, Cell *cell)
{
    double *materials = tool->multipliers.materials;

    if (materials[cell->material]) {
        return materials[cell->material];
    }
    DefaultDamage defaults = tool->multipliers.defaults;

    return SOLID == cell->type ? defaults.solid : defaults.creature;
}

static int16_t calculate_damage(Player *player, Tool *tool, Cell *target)
{
    State *states = player->state.map;

    double positive = states[STAMINA].current * 1.15;
    double negative = states[HUNGER].current + states[THIRST].current;

    double damage = (positive - (negative / 2)) / 20;

    if (tool) {
        damage *= tool_damage_multiplier(tool, target);
    }

    return (int16_t) ceil(damage);
}

static Hit calculate_hit(Player *player, Item *selected_item, Cell *target)
{
    uint16_t range = 1;
    Tool *tool = NULL;

    if (selected_item && TOOL == selected_item->type) {
        tool = &selected_item->tool;
        range = tool->range;
    }

    return (Hit) {
        .damage = calculate_damage(player, tool, target),
        .allowed_range = range,
    };
}

static void apply_hit_to_cell(Hit hit, Player *player, Cell *target, Point point)
{
    if (target->in_registry) {
        target = level_replace_cell_with_new(player->level, point);
    }

    target->state -= hit.damage;
    player->state.modifiers.dealt_damage += hit.damage;

    if (target->state <= 0) {
        sfx_play_break(target);
        level_set_hollow(player->level, point);
    }
}

static void apply_hit_to_item(Inventory *inventory, Item *item)
{
    if (item) {
        item->value -= rand_bool(0.3);

        if (0 >= item->value) {
            item_free(item);
            inventory_remove(inventory, item);
            sfx_play("item", "break");
        }
    }
}

EventError player_hit(InteractionEvent *event)
{
    Cell *target = event->cell;
    Player *player = event->player;
    Point point = event->point;

    if (SOLID != target->type && CREATURE != target->type) {
        return EE_CONTINUE;
    }

    Inventory *inventory = player->inventory;
    Item *selected_item = inventory_selected(inventory);

    Hit hit = calculate_hit(player, selected_item, target);

    if (hit.allowed_range >= point_distance(player->position.current, point)) {
        sfx_play_hit(target);
        apply_hit_to_cell(hit, player, target, point);
        apply_hit_to_item(inventory, selected_item);

        return EE_BREAK;
    }

    return EE_CONTINUE;
}
