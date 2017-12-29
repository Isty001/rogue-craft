#ifndef ROGUE_CRAFT_MATERIAL_H
#define ROGUE_CRAFT_MATERIAL_H


#include "util/color.h"


#define MATERIAL_NUM 6


typedef enum {
    MATERIAL_STONE,
    MATERIAL_DIRT,
    MATERIAL_SAND,
    MATERIAL_WATER,
    MATERIAL_WOOD,
    MATERIAL_SNOW
} Material;


extern const char *const MATERIAL_NAMES[];

extern const Style MATERIAL_STYLES[];


#endif
