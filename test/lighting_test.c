#include "unit_test.h"
#include "fixture.h"
#include "../src/level/lighting.h"


MU_TEST(test_lighting)
{
    Style style = 666;
    Level *level = fixture_level();
    Lighting *lighting = lighting_new(level, point_new(1, 1), 2, style);

    mu_assert_int_eq(style, level->cells[0][0]->style);
    mu_assert_int_eq(style, level->cells[0][2]->style);

    mu_assert_int_eq(FIXTURE_HOLLOW_STYLE, level->cells[0][1]->style);
    mu_assert_int_eq(FIXTURE_HOLLOW_STYLE, level->cells[1][0]->style);
    mu_assert_int_eq(FIXTURE_HOLLOW_STYLE, level->cells[1][1]->style);
    mu_assert_int_eq(FIXTURE_HOLLOW_STYLE, level->cells[1][2]->style);

    lighting_update(lighting, level, point_new(1, 0), 1);

    mu_assert_int_eq(style, level->cells[0][0]->style);
    mu_assert_int_eq(FIXTURE_SOLID_STYLE, level->cells[0][2]->style);

    lighting_free(lighting, level);

    mu_assert_int_eq(FIXTURE_SOLID_STYLE, level->cells[0][0]->style);

    fixture_level_free(level);
}

void run_lighting_test(void)
{
    TEST_NAME("Lighting");

    MU_RUN_TEST(test_lighting);

    MU_REPORT();
}
