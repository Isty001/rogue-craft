#ifndef ROGUE_CRAFT_RAY_SIGHT_H
#define ROGUE_CRAFT_RAY_SIGHT_H


#include "level/level.h"


typedef enum {
    UPDATED,
    REMAINED
} SightChange;

typedef enum {
    ALL,
    EDGES
} SightType;

typedef struct {
    uint16_t radius;
    uint16_t count;
    Point center;
    Point *points;
    SightType type;
} Sight;


Sight *sight_new(Level *level, Point center, uint16_t radius, SightType type);

bool sight_has(Sight *sight, Point point);

SightChange sight_update(Sight *sight, Level *level, Point center, uint16_t radius);

void sight_free(Sight *sight);


#endif
