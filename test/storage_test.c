#include "../src/storage/storage.h"
#include "unit_test.h"


MU_TEST(test_save)
{
    Player player;

    storage_save("test", &player);
}

void run_storage_test(void)
{
    storage_init("./test/fixture");

    TEST_NAME("Storage");
    MU_RUN_TEST(test_save);
    MU_REPORT();

    storage_rm_rf("./test/fixture/.rogue-craft");
}
