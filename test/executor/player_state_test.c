#include "../unit_test.h"
#include "../../src/worker/worker.h"


MU_TEST(test_movement)
{
    Player player;

    Message msg;
    msg.ptr = &player;

    execute_player_state(&msg);
}

void run_player_state_test(void)
{
    TEST_NAME("Player state");

    MU_RUN_TEST(test_movement);

    MU_REPORT();
}
