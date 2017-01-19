#include "unit_test.h"
#include "fixture.h"
#include "../src/player/player.h"


MU_TEST(test_new_player)
{
    Level *level = fixture_level();

    Player *player = player_new(level);
    Point prev = player->position.previous;
    Point curr = player->position.current;

    mu_assert(HOLLOW == level->cells[prev.y][prev.x]->type, "Starting position should Hollow");

    mu_assert_int_eq(prev.y, curr.y);
    mu_assert_int_eq(prev.x, curr.x);

    level_free(level);
    player_free(player);
}

void run_player_manager_test(void)
{
    TEST_NAME("Player Manager");

    MU_RUN_TEST(test_new_player);

    MU_REPORT();
}
