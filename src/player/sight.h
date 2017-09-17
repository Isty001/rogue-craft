#ifndef ROGUE_CRAFT_RAY_SIGHT_H
#define ROGUE_CRAFT_RAY_SIGHT_H


#include "level/level.h"


typedef enum {
    UPDATED,
    REMAINED
} SightChange;

typedef struct {
    uint16_t radius;
    Point center;
    uint16_t point_count;
    Point *points;
    Level *level;
} Sight;


Sight *sight_new(Level *level, Point center, uint16_t radius);

bool sight_has(Sight *sight, Point point);

SightChange sight_update(Sight *sight, Point center, uint16_t radius);

void sight_free(Sight *sight);


#endif
