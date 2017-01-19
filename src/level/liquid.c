#include "liquid.h"
#include "point.h"


#define LIQUID_VISIBLE '~'
#define LIQUID_HIDDEN ' '

#define PROBABILITY_OF_VISIBLE 0.75


void liquid_animate(Liquid *liquid)
{
    Cell *cell, **cells = liquid->cells;
    int i = 0;

    while ((cell = cells[i++])) {
        cell->chr = (char) (rand_bool(PROBABILITY_OF_VISIBLE) ? LIQUID_VISIBLE : LIQUID_HIDDEN);
    }
}

