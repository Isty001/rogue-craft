#ifndef ROGUE_CRAFT_RAY_SIGHT_H
#define ROGUE_CRAFT_RAY_SIGHT_H


#include "level/level.h"


typedef struct Sight Sight;


Sight *sight_new(Level *level, Point center, uint16_t radius);

bool sight_has(Sight *sight, Point point);

void sight_update(Sight *sight, Level *level, Point center, uint16_t radius);

void sight_free(Sight *sight);


#endif
