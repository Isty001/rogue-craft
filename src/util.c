#include "util.h"
#include "color.h"
#include "player/config.h"


#define color(nm)  \
    {.name = #nm, .value = COLOR_PAIR(nm)}

#define simple(nm) \
    {.name = #nm, .value = nm}

ConstLookup CONST_LOOKUP[] = {
    color(COLOR_PAIR_NONE),
    color(COLOR_PAIR_RED_F),
    color(COLOR_PAIR_GREEN_F),
    color(COLOR_PAIR_BLUE_F),
    color(COLOR_PAIR_YELLOW_F),
    color(COLOR_PAIR_GRAY_F),
    color(COLOR_PAIR_BROWN_F),
    color(COLOR_PAIR_CLARET_F),
    color(COLOR_PAIR_DARK_GREEN_F),

    simple(A_BOLD),
    simple(A_UNDERLINE),

    /** Attributes */
    simple(HEALTH),
    simple(STAMINA),
    simple(HUNGER),
    simple(THIRST),

    /** Materials */
    simple(STONE),
    simple(DIRT),
    simple(VOID),
    simple(WATER),
    simple(WOOD),
    {NULL, -1}
};

int constant(const char *search)
{
    int i = 0;
    char *name;

    if (search) {
        while ((name = CONST_LOOKUP[i].name)) {
            if (0 == strcmp(name, search)) {
                return CONST_LOOKUP[i].value;
            }
            i++;
        }
    }
    fatal("Constant [%s] not found\n", search);
}
