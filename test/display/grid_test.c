#include "../unit_test.h"
#include "../../src/ncurses/grid.h"


MU_TEST(test_selected)
{
    Grid grid = {
        .selected = point_new(0, 4),
        .size = size_new(5, 5),
        .bounds = bounds_from_size(grid.size)
    };

    grid_move_selected(&grid, NORTH);
    assert_point(grid.selected, 0, 4);

    grid_move_selected(&grid, SOUTH);
    assert_point(grid.selected, 1, 4);

    grid_move_selected(&grid, EAST);
    assert_point(grid.selected, 1, 4);
}

void run_grid_test(void)
{
    TEST_NAME("Grid");

    MU_RUN_TEST(test_selected);

    MU_REPORT();
}
