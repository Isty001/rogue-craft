#include "unit_test.h"
#include "../src/level/level.h"


MU_TEST(test_rand)
{
    Range range = {.from = 12, .to = 20};

    repeat(3,
           mu_assert(in_range(rand_in_range(range), range), "Should be in the range");
    )
}

void run_util_test(void)
{
    TEST_NAME("Util");

    MU_RUN_TEST(test_rand);

    MU_REPORT();
}
