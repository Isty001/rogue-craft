#include <getopt.h>
#include <stdio.h>
#include "dotenv.h"
#include "util/util.h"
#include "storage/storage.h"
#include "environment.h"


static struct option OPTIONS[] = {
    {"env", required_argument, 0, 'e'},
    {0}
};

static char BUFFER[PATH_MAX];


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
    sprintf(path, "%s/%s/environments/.env.%s", getenv("HOME"), DIR_APP_RELATIVE, env);
}

void env_setup(int argc, char **argv)
{
    char path[PATH_MAX];
    get_path(path, argc, argv);

    if (0 != env_load(path, false)) {
        fatal("Unable to setup the environment, please check '%s' permissions", path);
    }

    dir_create(getenv(ENV_DIR_CACHE));
}

char *env_json_resource_path(char *name)
{
    sprintf(BUFFER, "%s/json/%s", getenv(ENV_DIR_RESOURCES), name);

    return BUFFER;
}

char *env_cache_file(char *name)
{
    sprintf(BUFFER, "%s/%s.cache", getenv(ENV_DIR_CACHE), name);

    return BUFFER;
}
