#ifndef ROGUE_CRAFT_CELLULAR_H
#define ROGUE_CRAFT_CELLULAR_H


#include "level.h"


typedef Cell *(CellFactory)(Cell *current, uint16_t neighbours);


void cellular_generate(Level *level);


#endif
