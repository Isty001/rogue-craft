#ifndef ROGUE_CRAFT_ENVIRONMENT_H
#define ROGUE_CRAFT_ENVIRONMENT_H


#define ENV_DIR_CONFIG "DIR_CONFIG"
#define ENV_DIR_CACHE  "DIR_CACHE"



void env_setup(int argc, char **argv);

char *env_config(char *name);

char *env_cache_file(char *name);


#endif
