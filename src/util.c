#include "util.h"
#include "color.h"
#include "player/config.h"


#define pair(nm)  \
    {.name = #nm, .value = COLOR_PAIR(nm)}

#define constant(nm) \
    {.name = #nm, .value = nm}


ConstLookup CONST_LOOKUP[] = {
    pair(COLOR_PAIR_NONE),
    pair(COLOR_PAIR_RED_F),
    pair(COLOR_PAIR_GREEN_F),
    pair(COLOR_PAIR_BLUE_F),
    pair(COLOR_PAIR_YELLOW_F),
    pair(COLOR_PAIR_GRAY_F),
    constant(A_BOLD),
    constant(A_UNDERLINE),
    constant(HEALTH),
    constant(STAMINA),
    constant(HUNGER),
    constant(THIRST),
    constant(STONE),
    constant(DIRT),
    constant(VOID),
    constant(WATER),
    {NULL, -1}
};

