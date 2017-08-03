#include <getopt.h>
#include <stdio.h>
#include <dotenv.h>
#include "util/util.h"
#include "storage/storage.h"
#include "environment.h"


static struct option OPTIONS[] = {
    {"env", required_argument, 0, 'e'},
    {0}
};

static char BUFFER[500];

static void get_path(char *path, int argc, char **argv)
{
    int opt = 0;
    char env[30] = "prod";

    while (-1 != (opt = getopt_long(argc, argv, "e", OPTIONS, 0))) {
        if ('e' == opt) {
            sprintf(env, "%s", optarg);
            break;
        }
    }
    sprintf(path, "%s/%s/.env.%s", getenv("HOME"), DIR_ENV_RELATIVE, env);
}

void env_setup(int argc, char **argv)
{
    char path[300];
    get_path(path, argc, argv);

    env_load(path, false);

    dir_check(getenv(ENV_DIR_CACHE));
}

char *env_config_dir(char *name)
{
    sprintf(BUFFER, "%s/%s", getenv(ENV_DIR_CONFIG), name);

    return BUFFER;
}

char *env_cache_file(char *name)
{
    sprintf(BUFFER, "%s/%s.cache", getenv(ENV_DIR_CACHE), name);

    return BUFFER;
}
