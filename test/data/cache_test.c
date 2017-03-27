#include <list.h>
#include "../unit_test.h"
#include "../../src/storage/cache.h"


MU_TEST(test_exists)
{
    mu_assert(!cache_is_empty("test"), "Should not exist");

    Cache cache;
    cache_open(&cache, "test", sizeof(Point));

    mu_assert(!cache_is_empty("test"), "Should not exist");
    cache_add(&cache, &point_new(1, 1));
    cache_close(&cache);

    mu_assert(cache_is_empty("test"), "Should exist");
    mu_assert(NULL == cache.file, "");
}

static void assert_items(Point *point)
{
    static int i = 0;

    if (i == 0) {
        assert_point(*point, 10, 20);
    } else if (i == 1) {
        assert_point(*point, 20, 40);
    } else {
        fatal("Shouldn't be invoked more than twice");
    }
    i++;
}

MU_TEST(test_io)
{
    Cache cache;
    cache_open(&cache, "test", sizeof(Point));

    cache_add(&cache, &point_new(10, 20));
    cache_add(&cache, &point_new(20, 40));
    cache_foreach(&cache, (Reader) assert_items);

    cache_clear(&cache);
    cache_foreach(&cache, (Reader) assert_items);

    cache_delete("test");
    mu_assert(!file_exists(DIR_CACHE
                  "/test.cache"), "");

    cache_close(&cache);
}

MU_TEST(test_modified_date)
{
    Cache cache;
    cache_open(&cache, "test", sizeof(Point));

    mu_assert(cache_valid(&cache, time(NULL)), "Should be valid");
    mu_assert(!cache_valid(&cache, time(NULL) + 1), "Should not be valid");

    cache_close(&cache);
}

static void teardown(void)
{
    unlink(DIR_CACHE"/test.cache");
}

void run_cache_test(void)
{
    TEST_NAME("Cache");

    MU_SUITE_CONFIGURE(NULL, teardown);

    MU_RUN_TEST(test_exists);
    MU_RUN_TEST(test_io);
    MU_RUN_TEST(test_modified_date);

    MU_REPORT();
}
