#include "unit_test.h"
#include "fixture.h"
#include "../src/player/inventory.h"


static ItemPrototype TEST_TOOL = {
    .item = {
        .type = TOOL,
        .tool = {
            .range = 1,
            .damage = {
                .defaults = {.solid = 15}
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
        .attributes = {[STAMINA] = {.current = 80}}
    };
}

static void free_pixtures(Player *player)
{
    inventory_free(player->inventory);
    fixture_level_free(player->level);
}

MU_TEST(test_out_of_range)
{
    Player player = create_player();

    mu_assert_int_eq(PE_OUT_OF_RANGE, player_hit(&player, point_new(0, 2)));

    free_pixtures(&player);
}

MU_TEST(test_defaults)
{
    Player player = create_player();
    Cell *cell;

    mu_assert_int_eq(PE_DEALT_DAMAGE, player_hit(&player, point_new(0, 0)));

    cell = player.level->cells[0][0];
    mu_assert(false == cell->in_registry, "The cell hit should be cloned");

    /** 12 = (80 / 100) * 15  */
    mu_assert_double_eq(88, cell->state);

    free_pixtures(&player);
}

MU_TEST(test_material_and_tired_damage)
{
    Player player = create_player();
    player.attributes[HUNGER].current = 20;
    player.attributes[THIRST].current = 30;
    player.inventory->items[0]->tool.damage.materials[STONE] = 15;

    player_hit(&player, point_new(0, 0));

    /** 24 = (((80 / ((20 + 30) / 100)) / 100) * 15) */
    mu_assert_double_eq(76, player.level->cells[0][0]->state);

    free_pixtures(&player);
}

MU_TEST(test_bare_hands)
{
    Player player = create_player();
    player.inventory->selected = 1;

    player_hit(&player, point_new(0, 0));

    /** 8 = 80 / 10 */
    mu_assert_double_eq(92, player.level->cells[0][0]->state);

    free_pixtures(&player);
}

void run_player_hit_test(void)
{
    TEST_NAME("Player Hit");

    MU_RUN_TEST(test_out_of_range);
    MU_RUN_TEST(test_defaults);
    MU_RUN_TEST(test_material_and_tired_damage);
    MU_RUN_TEST(test_bare_hands);

    MU_REPORT();
}
