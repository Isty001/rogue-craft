#ifndef ROGUE_CRAFT_LIGHTING_H
#define ROGUE_CRAFT_LIGHTING_H


#include "player/sight.h"


typedef struct Lighting {
    Sight *sight;
    Style style;
} Lighting;


Lighting *lighting_new(Level *level, Point source, uint16_t radius, Style style);

void lighting_free(Lighting *lighting);


#endif
