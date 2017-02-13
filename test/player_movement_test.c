#include "unit_test.h"
#include "../src/player/player.h"
#include "fixture.h"


static void assert_position(Player player, int y, int x)
{
    Point position = player.position.current;

    mu_assert_double_eq(y, position.y);
    mu_assert_double_eq(x, position.x);
}

MU_TEST(test_movement)
{
    Player player;
    player.level = fixture_level();
    player.cell.prototype.type = PLAYER;
    player.cell.prototype.in_registry = true;
    player.cell.previous = &player.level->registry.hollow.cells[0];
    player.position.previous = point_new(1, 1);
    player.position.current = point_new(1, 1);

    repeat(4,
           player_move(&player, NORTH);
    )

    assert_position(player, 0, 1);

    player_move(&player, SOUTH_EAST);
    repeat(3,
           player_move(&player, WEST);
    )
    assert_position(player, 1, 0);

    Cell ***cells = player.level->cells;

    mu_assert(PLAYER == cells[1][0]->type, "The player should be there");

    mu_assert(
        HOLLOW == cells[1][1]->type &&
        HOLLOW == cells[1][2]->type,
        "Previous positions should be restored"
    );

    fixture_level_free(player.level);
}

MU_TEST(test_eyesight)
{
    Player player;
    player.position.current = point_new(1, 0);

    Point *visible = alloc(5 * sizeof(Point));
    player.sight.radius = 1;
    player.sight.visible = visible;
    player.sight.visible_count = 0;

    player.level = fixture_level();

    player_calculate_sight(&player);

    mu_assert(point_eq(point_new(1, 0), visible[0]), "");
    mu_assert(point_eq(point_new(1, 1), visible[1]), "");
    mu_assert(point_eq(point_new(1, 2), visible[2]), "");

    mu_assert_int_eq(3, player.sight.visible_count);

    fixture_level_free(player.level);
    free(player.sight.visible);
}

void run_player_movement_test(void)
{
    TEST_NAME("Player Movement");

    MU_RUN_TEST(test_movement);
    MU_RUN_TEST(test_eyesight);

    MU_REPORT();
}
