#include "unit_test.h"


MU_TEST(test_rand)
{
    Range range = {.from = 12, .to = 20};

    repeat(3,
           mu_assert(in_range(rand_in_range(range), range), "Should be in the cfg");
    )
}

MU_TEST(test_max)
{
    mu_assert_int_eq(10, max(10, 5));
    mu_assert_int_eq(32, max(11, 32));
}

void run_util_test(void)
{
    TEST_NAME("Util");

    MU_RUN_TEST(test_rand);
    MU_RUN_TEST(test_max);

    MU_REPORT();
}
