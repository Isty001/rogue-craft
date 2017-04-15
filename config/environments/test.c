#include <stdlib.h>
#include <stdbool.h>
#include "../../src/environment.h"


void ENV_SETUP_ENTRY_POINT(void)
{
    setenv(ENV_DIR_CONFIG, "./test/fixture/config", false);
    setenv(ENV_DIR_CACHE, "./test/fixture/cache", false);
}
