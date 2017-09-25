#ifndef ROGUE_CRAFT_MATERIAL_H
#define ROGUE_CRAFT_MATERIAL_H


#define MATERIAL_NUM 7
#define MATERIAL_LIQUID_NUM 1


typedef enum {
    STONE,
    DIRT,
    SAND,
    WATER,
    WOOD,
    SNOW
} Material;


static inline const char *material_to_str(Material material)
{
    switch (material) {
        case STONE:
            return "stone";
        case WOOD:
            return "wood";
        case DIRT:
            return "dirt";
        case SNOW:
            return "snow";
        case SAND:
            return "sand";
        case WATER:
            return "water";
    }
    return NULL;
}

static inline Style material_style(Material material)
{
    switch (material) {
        case STONE:
            return COLOR_PAIR(COLOR_GRAY_F);
        case WOOD:
            return COLOR_PAIR(COLOR_BROWN_F);
        case DIRT:
            return COLOR_PAIR(COLOR_DARK_BROWN_F);
        case SNOW:
            return COLOR_PAIR(COLOR_WHITE_F);
        case SAND:
            return COLOR_PAIR(COLOR_YELLOW_F);
        case WATER:
            return COLOR_PAIR(COLOR_BLUE_F);
    }
    return 0;
}

#endif
