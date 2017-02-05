#include "unit_test.h"
#include "../src/level/cell.h"
#include "../src/level/level.h"
#include "fixture.h"


MU_TEST(test_clone)
{
    Cell cell, *new;
    cell.in_registry = true;

    new = cell_clone(&cell);
    mu_assert(false == new->in_registry, "This is not in a registry");
}

MU_TEST(test_clone_at_level)
{
    Level *level = fixture_level();

    Cell *new = level_replace_cell_with_new(level, point_new(0, 0));
    mu_assert_int_eq(SOLID, new->type);

    fixture_level_free(level);
}

void run_cell_test(void)
{
    TEST_NAME("Cell");

    MU_RUN_TEST(test_clone);
    MU_RUN_TEST(test_clone_at_level);

    MU_REPORT();
}
