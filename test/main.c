#include <stdlib.h>
#include <time.h>
#include "../src/color.h"

void run_player_movement_test(void);
void run_point_test(void);
void run_util_test(void);
void run_player_manager_test(void);
void run_camera_test(void);
void run_consumable_test(void);
void run_buffer_test(void);


int main(void)
{
    srand((unsigned int) time(NULL));

    run_player_movement_test();
    run_point_test();
    run_util_test();
    run_player_manager_test();
    run_camera_test();
    run_consumable_test();
    run_buffer_test();

    return 0;
}
