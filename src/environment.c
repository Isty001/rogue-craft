#include <getopt.h>
#include <stdio.h>
#include <dlfcn.h>
#include "util.h"
#include "storage/storage.h"
#include "environment.h"


typedef void (*EnvSetup)(void);


static struct option OPTIONS[] = {
    {"env", required_argument, 0, 'e'},
    {0}
};

static void get_path(char *path, int argc, char **argv)
{
    int opt = 0;
    char env[30] = "production";

    while (-1 != (opt = getopt_long(argc, argv, "e", OPTIONS, 0))) {
        if ('e' == opt) {
            sprintf(env, "%s", optarg);
            break;
        }
    }
    sprintf(path, "%s/%s.so", DIR_ENV_CONFIG, env);
}

static void *load(char *path)
{
    void *handle = dlopen(path, RTLD_NOW);

    if (!handle) {
        fatal("Unable to load Environment config [%s]", dlerror());
    }

    return handle;
}

void env_setup(int argc, char **argv)
{
    char path[200];
    get_path(path, argc, argv);

    void *handle = load(path);
    EnvSetup setup = dlsym(handle, stringify(ENV_SETUP_ENTRY_POINT));

    if (!setup) {
        fatal("Environment config [%s] must have an EnvSetup load point", path);
    }
    setup();

    dlclose(handle);
    dir_check(getenv(ENV_DIR_CACHE));
}

char *env_config(char *name)
{
    static char config[30];

    sprintf(config, "%s/%s", getenv(ENV_DIR_CONFIG), name);

    return config;
}

char *env_cache_file(char *name)
{
    static char cache[300];

    sprintf(cache, "%s/%s.cache", getenv(ENV_DIR_CACHE), name);

    return cache;
}
