#ifndef ROGUE_CRAFT_MATERIAL_H
#define ROGUE_CRAFT_MATERIAL_H


#define MATERIAL_NUM 7
#define MATERIAL_LIQUID_NUM 1


typedef enum {
    MATERIAL_STONE,
    MATERIAL_DIRT,
    MATERIAL_SAND,
    MATERIAL_WATER,
    MATERIAL_WOOD,
    MATERIAL_SNOW
} Material;


static inline const char *material_to_str(Material material)
{
    switch (material) {
        case MATERIAL_STONE:
            return "stone";
        case MATERIAL_WOOD:
            return "wood";
        case MATERIAL_DIRT:
            return "dirt";
        case MATERIAL_SNOW:
            return "snow";
        case MATERIAL_SAND:
            return "sand";
        case MATERIAL_WATER:
            return "water";
    }
    return NULL;
}

static inline Style material_style(Material material)
{
    switch (material) {
        case MATERIAL_STONE:
            return COLOR_PAIR(COLOR_GRAY_F);
        case MATERIAL_WOOD:
            return COLOR_PAIR(COLOR_BROWN_F);
        case MATERIAL_DIRT:
            return COLOR_PAIR(COLOR_DARK_BROWN_F);
        case MATERIAL_SNOW:
            return COLOR_PAIR(COLOR_WHITE_F);
        case MATERIAL_SAND:
            return COLOR_PAIR(COLOR_YELLOW_F);
        case MATERIAL_WATER:
            return COLOR_PAIR(COLOR_BLUE_F);
    }
    return 0;
}

#endif
