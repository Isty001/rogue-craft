#ifndef ROGUECRAFT_FIXTURE_H
#define ROGUECRAFT_FIXTURE_H


#include "../src/level/level.h"


/**
 * Level cells will look like this:
 *
 * S H S
 * H H H
 */
Level *fixture_level(void);

void fixture_level_free(Level *level);


#endif
