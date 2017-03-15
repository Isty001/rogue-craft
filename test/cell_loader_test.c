#include "unit_test.h"
#include "../src/level/cell.h"
#include "../config/config.h"
#include "../src/storage/cache.h"


static void assert_common(const Cell *cell)
{
    mu_assert_int_eq(STONE, cell->material);
    mu_assert_int_eq(SOLID, cell->type);
    mu_assert_int_eq(COLOR_PAIR(COLOR_PAIR_GRAY_F), cell->style);
}

static void assert_cells()
{
    const Cell *cobble = cell_get("stone.cobble");
    assert_wchar("#", cobble->chr, 1);

    const Cell *mossy = cell_get("stone.mossy");
    assert_wchar("*", mossy->chr, 1);

    assert_common(cobble);
    assert_common(mossy);
}

MU_TEST(test_load)
{
    mu_assert(!cache_exists(CACHE_CONFIG_CELL), "");

    cell_load();
    assert_cells();
    cell_unload();

    mu_assert(cache_exists(CACHE_CONFIG_CELL), "");

    rename(DIR_CONFIG_CELLS"/cell.json", DIR_CONFIG"/tmp/cell.json");

    cell_load();
    assert_cells();
    cell_unload();

    rename(DIR_CONFIG"/tmp/cell.json", DIR_CONFIG_CELLS"/cell.json");
}

void run_cell_loader_test(void)
{
    TEST_NAME("Cell Loader");

    MU_RUN_TEST(test_load);

    MU_REPORT();
}
