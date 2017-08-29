#include "memory/memory.h"
#include "util/environment.h"
#include "util/logger.h"
#include "../unit_test.h"


static const char *EXPECTED = "[ALERT] - 2017-08-29 22:23:06 - Test Alert 101 - Hello\n"
    "[ERROR] - 2017-08-29 22:23:06 - Test Error";


MU_TEST(test_logger)
{
    assert_file_exists(getenv(ENV_LOG_FILE));

    log_debug("Test Debug %d - %s", 10, "Hello");
    log_info("Test Info");
    log_alert("Test Alert %d - %s", 101, "Hello");
    log_error("Test Error");

    FILE *file = file_open(getenv(ENV_LOG_FILE), "r");
    char *content = file_read(file);

    mu_assert(NULL == strstr(content, "DEBUG"), "");
    mu_assert(NULL == strstr(content, "INFO"), "");

    mu_assert(NULL != strstr(content, "ALERT"), "");
    mu_assert(NULL != strstr(content, "ERROR"), "");
    mu_assert(NULL != strstr(content, "Test Alert 101 - Hello"), "");

    mem_dealloc(content);
}

void run_logger_test(void)
{
    TEST_NAME("Logger");

    MU_RUN_TEST(test_logger);

    MU_REPORT();
}
