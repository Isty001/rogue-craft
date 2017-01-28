#include "unit_test.h"
#include "../src/level/level.h"


MU_TEST(test_rand)
{
    Range range = {.from = 12, .to = 20};

    repeat(3,
           mu_assert(in_range(rand_in_range(range), range), "Should be in the range");
    )
}

MU_TEST(test_min_max)
{
    mu_assert_int_eq(2, min(2, 3));
    mu_assert_int_eq(2, min(2, 2));
    mu_assert_int_eq(2, min(3, 2));

    mu_assert_int_eq(3, max(2, 3));
    mu_assert_int_eq(2, max(2, 2));
    mu_assert_int_eq(3, max(3, -2));
}

void run_util_test(void)
{
    TEST_NAME("Util");

    MU_RUN_TEST(test_rand);
    MU_RUN_TEST(test_min_max);

    MU_REPORT();
}
