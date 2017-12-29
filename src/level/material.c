#include "material.h"


const char *const MATERIAL_NAMES[] = {
    [MATERIAL_STONE] = "stone",
    [MATERIAL_DIRT] = "dirt",
    [MATERIAL_SAND] = "sand",
    [MATERIAL_WATER] = "water",
    [MATERIAL_WOOD] = "wood",
    [MATERIAL_SNOW] = "snow"
};

const Style MATERIAL_STYLES[] = {
    [MATERIAL_STONE] =  COLOR_PAIR(COLOR_GRAY_F),
    [MATERIAL_WOOD] = COLOR_PAIR(COLOR_BROWN_F),
    [MATERIAL_DIRT] = COLOR_PAIR(COLOR_DARK_BROWN_F),
    [MATERIAL_SNOW] = COLOR_PAIR(COLOR_WHITE_F),
    [MATERIAL_SAND] = COLOR_PAIR(COLOR_YELLOW_F),
    [MATERIAL_WATER] = COLOR_PAIR(COLOR_BLUE_F),
};

