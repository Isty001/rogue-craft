#include <getopt.h>
#include <stdio.h>
#include <dlfcn.h>
#include "util.h"
#include "storage/storage.h"
#include "environment.h"


#define convert(a) #a
#define stringify(a) convert(a)


typedef void (*EnvSetup)(void);


static struct option OPTIONS[] = {
    {"env", required_argument, 0, 'e'},
    {0}
};

static char BUFFER[400];

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
    sprintf(path, "%s/%s.so", DIR_ENV, env);
}

static void *load_object(char *path)
{
    void *handle = dlopen(path, RTLD_NOW);

    if (!handle) {
        fatal("%s", dlerror());
    }

    return handle;
}

static void setup(void *handle)
{
    EnvSetup entry_point = dlsym(handle, stringify(ENV_SETUP_ENTRY_POINT));

    if (!entry_point) {
        fatal("%s", dlerror());
    }
    entry_point();
}

void env_setup(int argc, char **argv)
{
    char path[200];
    get_path(path, argc, argv);

    void *handle = load_object(path);
    setup(handle);
    dlclose(handle);

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
