#include "level/level.h"
#include "../unit_test.h"
#include "../fixture.h"


MU_TEST(test_bounds)
{
    Level *level = fixture_level();

    mu_assert(level_in_bounds(level, point_new(1, 0)), "Should be in bounds");
    mu_assert(level_in_bounds(level, point_new(1, 2)), "Should be in bounds");

    mu_assert(false == level_in_bounds(level, point_new(-1, 2)), "Should not be in bounds");
    mu_assert(false == level_in_bounds(level, point_new(1, 3)), "Should not be in bounds");

    fixture_level_free(level);
}

void run_level_test(void)
{
    TEST_NAME("Level");

    MU_RUN_TEST(test_bounds);

    MU_REPORT();
}
