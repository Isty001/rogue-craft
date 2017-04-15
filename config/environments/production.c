#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <stdio.h>
#include "../../src/environment.h"


static void set_path(char *home, size_t home_len, char *sub_dir, char *env)
{
    char path[home_len + 30];

    sprintf(path, "%s/%s", home, sub_dir);
    setenv(env, path, false);
}

void ENV_SETUP_ENTRY_POINT(void)
{
    char *home = getenv("HOME");
    size_t home_len = strlen(home);

    set_path(home, home_len, ".config/rogue-craft", ENV_DIR_CONFIG);
    set_path(home, home_len, ".cache/rogue-craft", ENV_DIR_CACHE);
}

