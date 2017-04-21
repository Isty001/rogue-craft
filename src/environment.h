#ifndef ROGUE_CRAFT_ENVIRONMENT_H
#define ROGUE_CRAFT_ENVIRONMENT_H


#define ENV_SETUP_ENTRY_POINT envsetup
#define ENV_DIR_CONFIG "DIR_CONFIG"
#define ENV_DIR_CACHE  "DIR_CACHE"
#define ENV_NCURSES_INACTIVE  "NCURSES_DISABLED"


#define env_has(var) (bool)getenv(var)


void env_setup(int argc, char **argv);

char *env_config_dir(char *name);

char *env_cache_file(char *name);


#endif
