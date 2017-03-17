#ifndef ROGUE_CRAFT_RAY_SIGHT_H
#define ROGUE_CRAFT_RAY_SIGHT_H


#include "level/level.h"


typedef enum {
    SIGHT_UPDATED,
    SIGHT_REMAINED
} SightChange;


typedef struct {
    uint16_t radius;
    uint16_t count;
    Point center;
    Point *points;
} Sight;


Sight *sight_new(Level *level, Point center, uint16_t radius);

bool sight_has(Sight *sight, Point point);

SightChange sight_update(Sight *sight, Level *level, Point center, uint16_t radius);

void sight_free(Sight *sight);


#endif
