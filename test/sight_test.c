#include "unit_test.h"
#include "../src/sight.h"
#include "fixture.h"


MU_TEST(test_sight_all)
{
    Level *level = fixture_level();
    Sight *sight = sight_new(level, point_new(1, 0), 2, ALL);

    mu_assert(sight_has(sight, point_new(0, 0)), "");
    mu_assert(sight_has(sight, point_new(0, 1)), "");
    mu_assert(sight_has(sight, point_new(1, 0)), "");
    mu_assert(sight_has(sight, point_new(1, 1)), "");

    mu_assert(!sight_has(sight, point_new(0, 2)), "");

    mu_assert_int_eq(UPDATED, sight_update(sight, point_new(1, 1), 2));
    mu_assert(sight_has(sight, point_new(0, 2)), "");

    sight_free(sight);
    fixture_level_free(level);
}

void run_sight_test(void)
{
    TEST_NAME("Sight");

    MU_RUN_TEST(test_sight_all);

    MU_REPORT();
}
