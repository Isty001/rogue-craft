#include "unit_test.h"
#include "fixture.h"
#include "../src/player/inventory.h"


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
    Inventory *inventory = inventory_new(2);
    inventory->items[0] = item_clone(&TEST_TOOL);

    return inventory;
}

static Player create_player(void)
{
    return (Player) {
        .level = fixture_level(),
        .inventory = create_inventory(),
        .position = {.current = point_new(1, 0), .previous = point_new(1000, 1000)},
        .attributes.state = {[STAMINA] = {.current = 80}}
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

    mu_assert_int_eq(PE_OUT_OF_RANGE, player_hit(&player, point_new(0, 2)));

    player.inventory->items[0]->tool.range = 2;
    mu_assert_int_eq(PE_DEALT_DAMAGE, player_hit(&player, point_new(0, 2)));

    free_fixtures(&player);
}

MU_TEST(test_defaults)
{
    Player player = create_player();
    Cell *cell;

    mu_assert_int_eq(PE_DEALT_DAMAGE, player_hit(&player, point_new(0, 0)));

    cell = player.level->cells[0][0];
    mu_assert(false == cell->in_registry, "The cell hit should be cloned");

    mu_assert_double_eq(94.72, cell->state);
    mu_assert_int_eq(5.38, player.attributes.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_material_and_tired_damage)
{
    Player player = create_player();
    player.attributes.state[HUNGER].current = 20;
    player.attributes.state[THIRST].current = 30;
    player.inventory->items[0]->tool.multipliers.materials[STONE] = 2;

    mu_assert_int_eq(PE_DEALT_DAMAGE, player_hit(&player, point_new(0, 0)));

    mu_assert_double_eq(92.9, player.level->cells[0][0]->state);
    mu_assert_int_eq(7.1, player.attributes.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_bare_hands)
{
    Player player = create_player();
    player.inventory->selected = 1;

    mu_assert_int_eq(PE_DEALT_DAMAGE, player_hit(&player, point_new(0, 0)));

    mu_assert_double_eq(95.2, player.level->cells[0][0]->state);
    mu_assert_double_eq(4.8, player.attributes.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_invalid_cell_type)
{
    Player player = create_player();
    player.level->cells[0][0]->type = HOLLOW;

    mu_assert_int_eq(PE_INVALID_CELL, player_hit(&player, point_new(0, 0)));

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
