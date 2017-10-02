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
    simple(STATE_HEALTH),
    simple(STATE_STAMINA),
    simple(STATE_HUNGER),
    simple(STATE_THIRST),

    /** Materials */
    simple(MATERIAL_STONE),
    simple(MATERIAL_DIRT),
    simple(MATERIAL_SAND),
    simple(MATERIAL_WATER),
    simple(MATERIAL_WOOD),
    simple(MATERIAL_SNOW),

    /** CellType */
    simple(CELL_SOLID),
    simple(CELL_CREATURE),
    simple(CELL_HOLLOW),
    simple(CELL_PLAYER),
    simple(CELL_LIQUID),
    {"ITEM", CELL_ITEM},

    /** LevelType */
    simple(LEVEL_CELLULAR),

    {NULL, -1}
};

uint64_t constant(const char *search)
{
    int i = 0;
    char *name;

    while ((name = CONST_LOOKUP[i].name)) {
        if (0 == strcmp(search, name)) {
            return CONST_LOOKUP[i].value;
        }
        i++;
    }
    fatal("Constant [%s] not found\n", search);
}

uint64_t constant_prefixed(const char *prefix, const char *name)
{
    char search[strlen(prefix) + strlen(name) + 1];
    sprintf(search, "%s_%s", prefix, name);

    return constant(search);
}
