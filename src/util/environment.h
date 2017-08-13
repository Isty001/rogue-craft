#ifndef ROGUE_CRAFT_ENVIRONMENT_H
#define ROGUE_CRAFT_ENVIRONMENT_H


#include <stdlib.h>


#define ENV_DIR_RESOURCES "DIR_RESOURCES"
#define ENV_DIR_CACHE  "DIR_CACHE"
#define ENV_DIR_SAVED_GAMES "DIR_SAVED_GAMES"
#define ENV_NCURSES_INACTIVE  "NCURSES_DISABLED"
#define ENV_DEBUG_MODE "DEBUG_MODE"


void env_setup(int argc, char **argv);

/**
 * @return {ENV_DIR_RESOURCES}/json/name
 */
char *env_json_resource_path(char *name);

char *env_cache_file(char *name);


#endif
