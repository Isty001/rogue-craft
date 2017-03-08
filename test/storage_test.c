#include "unit_test.h"
#include "../src/storage/storage.h"


MU_TEST(test_file_exists)
{
    mu_assert(file_exists("./main.c"), "");
    mu_assert(!file_exists("./something-unknown.c"), "");
}

MU_TEST(test_file_size)
{
    char *path = "./test/fixture/test_size";
    FILE *file = file_open(path, "wr");
    fwrite("Hello", 5, 1, file);

    mu_assert_int_eq(5, file_size(file));

    fclose(file);
    unlink(path);
}

void run_storage_test(void)
{
    TEST_NAME("Storage");

    MU_RUN_TEST(test_file_exists);
    MU_RUN_TEST(test_file_size);

    MU_REPORT();
}
