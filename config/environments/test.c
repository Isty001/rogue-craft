#include <stdlib.h>
#include <stdbool.h>
#include "../../src/environment.h"
#include "../../test/fixture.h"


void ENV_SETUP_ENTRY_POINT(void)
{
    setenv(ENV_DIR_CONFIG, "./test/fixture/config", false);
    setenv(ENV_DIR_CACHE, "./test/fixture/cache", false);
    setenv(ENV_NCURSES_INACTIVE, "inactive", false);
    setenv(ENV_DIR_FIXTURE, "./test/fixture", false);
    setenv(ENV_DEBUG, "debug", false);
}
