#include "unit_test.h"
#include "../src/storage/storage.h"


#define STORAGE_FILE FIXTURE_DIR"/storage.txt"


static void teardown(void)
{
    unlink(STORAGE_FILE);
}

MU_TEST(test_create)
{
    Buffer buff;

    mu_assert(SE_OK == buffer_new(&buff, STORAGE_FILE), "This should be SE_OK");
    buffer_free(&buff);

    mu_assert(SE_IO == buffer_new(&buff, "non-existent/path"), "This should throw SE_IO");
}

MU_TEST(test_append)
{
    Buffer buff;

    buffer_new(&buff, STORAGE_FILE);
    buffer_append(&buff, "buff");

    assert_string("buff", buff.content);

    buffer_append(&buff, "\nHello");
    buffer_append(&buff, "\nMore t3xT");

    assert_string("buff\nHello\nMore t3xT", buff.content);

    buffer_free(&buff);
}

MU_TEST(test_persist)
{
    Buffer buff;

    char *expected = "Hello!\nHi!\n";
    size_t len = strlen(expected);

    buffer_new(&buff, STORAGE_FILE);

    buffer_append(&buff, expected);
    buffer_persist(&buff);
    buffer_free(&buff);

    FILE *file = fopen(STORAGE_FILE, "r");
    char actual[len];

    fread(&actual, len, 1, file);

    actual[len] = '\0';
    assert_string(expected, actual);

    fclose(file);
}

void run_buffer_test(void)
{
    TEST_NAME("Buffer");

    MU_SUITE_CONFIGURE(NULL, teardown);
    MU_RUN_TEST(test_create);
    MU_RUN_TEST(test_append);
    MU_RUN_TEST(test_persist);

    MU_REPORT();
}
