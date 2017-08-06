#include <mem_pool.h>
#include <list.h>
#include "util.h"
#include "color.h"
#include "player/player.h"


#define color(nm)  \
    {.name = #nm, .value = COLOR_PAIR(nm)}

#define simple(nm) \
    {.name = #nm, .value = nm}

ConstLookup CONST_LOOKUP[] = {
    color(COLOR_NONE),
    color(COLOR_RED_F),
    color(COLOR_GREEN_F),
    color(COLOR_BLUE_F),
    color(COLOR_YELLOW_F),
    color(COLOR_GRAY_F),
    color(COLOR_BROWN_F),
    color(COLOR_CLARET_F),
    color(COLOR_DARK_GREEN_F),
    color(COLOR_DARK_GRAY_F),
    color(COLOR_GREEN_B),
    color(COLOR_DARK_GREEN_B),
    color(COLOR_BROWN_B),
    color(COLOR_YELLOW_B),
    color(COLOR_ORANGE_B),
    color(COLOR_RED_F_B),
    color(COLOR_WHITE_B),
    color(COLOR_GRAY_B),
    color(COLOR_DARK_GRAY_B),
    color(COLOR_DARK_BROWN_B),
    color(COLOR_DARK_BROWN_F),

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
    simple(SAND),
    simple(WATER),
    simple(WOOD),
    simple(SNOW),

    /** CellType */
    simple(SOLID),
    simple(CREATURE),
    simple(HOLLOW),
    simple(PLAYER),
    simple(LIQUID),
    simple(ITEM),

    /** LevelType */
    simple(CELLULAR),

    {NULL, -1}
};

uint64_t constant(const char *search)
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
