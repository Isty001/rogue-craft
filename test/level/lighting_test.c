#include "../unit_test.h"
#include "../fixture.h"
#include "player/camera.h"
#include "../../src/level/lighting/lighting.h"


MU_TEST(test_create)
{
    Level *level = fixture_level();
    Lighting *lighting = lighting_new(level, point_new(1, 1), 2, 100);

    mu_assert(lighting == level->lightings->head(level->lightings), "");
    mu_assert_int_eq(100, lighting->style);
    assert_point(lighting->sight->center, 1, 1);

    fixture_level_free(level);
}

void run_lighting_test(void)
{
    TEST_NAME("Lighting");

    MU_RUN_TEST(test_create);

    MU_REPORT();
}
