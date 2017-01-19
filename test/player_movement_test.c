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
    player.cell.prototype.is_prototype = true;
    player.cell.occupied = &player.level->registry.hollow.cells[0];
    player.position.previous = point_new(1, 1);
    player.position.current = point_new(1, 1);

    repeat(4,
           player_move(&player, NORTH);
    )
    assert_position(player, 0, 1);

    player_move(&player, SOUTH);
    player_move(&player, EAST);

    repeat(3,
           player_move(&player, WEST);
           player_position_on_level(&player);
    )
    assert_position(player, 1, 0);

    Cell ***cells = player.level->cells;

    mu_assert(PLAYER == cells[1][0]->type, "The player should be there");

    mu_assert(
        HOLLOW == cells[1][1]->type &&
        HOLLOW == cells[1][2]->type,
        "Previous positions should be restored"
    );

    level_free(player.level);
}

MU_TEST(test_eyesight)
{
    Player player;
    player.position.current = point_new(10, 10);
    player.eyesight = 5;

    mu_assert(player_can_see(&player, 10, 15), "Player should see this point");
    mu_assert(false == player_can_see(&player, 20, 10), "Player should not see this");
}

void run_player_movement_test(void)
{
    TEST_NAME("Player Movement");

    MU_RUN_TEST(test_movement);
    MU_RUN_TEST(test_eyesight);

    MU_REPORT();
}
