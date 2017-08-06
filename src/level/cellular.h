#ifndef ROGUE_CRAFT_CELLULAR_H
#define ROGUE_CRAFT_CELLULAR_H


#include "level.h"


typedef Cell *(CellFactory)(Cell *current, uint16_t neighbours);


void cellular_generate(Cell ***cells, Size size, CellFactory factory);

void cellular_generate_level(Level *level);


#endif
