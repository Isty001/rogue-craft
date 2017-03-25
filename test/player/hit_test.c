#include "../unit_test.h"
#include "../fixture.h"
#include "../../src/player/inventory.h"


static ItemPrototype TEST_TOOL = {
    .item = {
        .type = TOOL,
        .tool = {
            .range = 1,
            .multipliers = {
                .defaults = {.solid = 1.1}
            }
        }
    }
};


static Inventory *create_inventory(void)
{
    Inventory *inv = inventory_new(2);
    inv->items->append(inv->items, item_clone(&TEST_TOOL));

    return inv;
}

static Player create_player(void)
{
    return (Player) {
        .level = fixture_level(),
        .inventory = create_inventory(),
        .position = {.current = point_new(1, 0), .previous = point_new(1000, 1000)},
        .state.attributes = {[STAMINA] = {.current = 80}}
    };
}

static InteractionEvent create_event(Player *player, Point point)
{
    return (InteractionEvent) {
        .player = player,
        .cell = player->level->cells[point.y][point.x],
        .point =point
    };
}

static void free_fixtures(Player *player)
{
    inventory_free(player->inventory);
    fixture_level_free(player->level);
}

MU_TEST(test_range)
{
    Player player = create_player();
    List *items = player.inventory->items;
    InteractionEvent event = create_event(&player, point_new(0, 2));

    player_hit(&event);

    Item *tool = items->head(items);
    tool->tool.range = 2;

    player_hit(&event);
    mu_assert_double_eq(94.72, player.level->cells[0][2]->state);
    mu_assert_double_eq(5.28, player.state.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_defaults)
{
    Player player = create_player();
    Cell *cell;

    InteractionEvent event = create_event(&player, point_new(0, 0));

    player_hit(&event);

    cell = player.level->cells[0][0];
    mu_assert(false == cell->in_registry, "The cell hit should be cloned");

    mu_assert_double_eq(94.72, cell->state);
    mu_assert_int_eq(5.38, player.state.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_material_and_tired_damage)
{
    Player player = create_player();
    List *items = player.inventory->items;
    Item *tool;

    player.state.attributes[HUNGER].current = 20;
    player.state.attributes[THIRST].current = 30;
    tool = items->head(items);
    tool->tool.multipliers.materials[STONE] = 2;

    InteractionEvent event = create_event(&player, point_new(0, 0));

    player_hit(&event);

    mu_assert_double_eq(92.9, player.level->cells[0][0]->state);
    mu_assert_int_eq(7.1, player.state.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_bare_hands)
{
    Player player = create_player();
    InteractionEvent event = create_event(&player, point_new(0, 0));
    player.inventory->selected = 1;

    player_hit(&event);

    mu_assert_double_eq(95.2, player.level->cells[0][0]->state);
    mu_assert_double_eq(4.8, player.state.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_invalid_cell_type)
{
    Player player = create_player();
    InteractionEvent event = create_event(&player, point_new(0, 0));
    Cell *cell = player.level->cells[0][0];
    cell->type = HOLLOW;

    player_hit(&event);
    mu_assert(cell->in_registry, "Shouldn't be touched");

    free_fixtures(&player);
}

void run_player_hit_test(void)
{
    TEST_NAME("Hit");

    MU_RUN_TEST(test_range);
    MU_RUN_TEST(test_defaults);
    MU_RUN_TEST(test_material_and_tired_damage);
    MU_RUN_TEST(test_bare_hands);
    MU_RUN_TEST(test_invalid_cell_type);

    MU_REPORT();
}
