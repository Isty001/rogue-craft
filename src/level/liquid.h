#ifndef ROGUECRAFT_LIQUID_H
#define ROGUECRAFT_LIQUID_H


#include "level.h"


typedef struct {
    Cell **cells;
} Liquid;


void liquid_animate(Liquid *liquid);


#endif
