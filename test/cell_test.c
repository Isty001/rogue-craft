#include "unit_test.h"
#include "../src/level/cell.h"


MU_TEST(test_clone)
{
    Cell cell, *new;
    cell.in_registry = true;

    new = cell_clone(&cell);
    mu_assert(false == new->in_registry, "This is not in a registry");
}

void run_cell_test(void)
{
    TEST_NAME("Cell");

    MU_RUN_TEST(test_clone);

    MU_REPORT();
}
