#ifndef ROGUECRAFT_DEFAULTS_H
#define ROGUECRAFT_DEFAULTS_H


#include "../src/color.h"
#include "../src/level/cell.h"
#include "../src/level/level.h"
#include "../src/player/player.h"
#include "../src/player/item.h"
#include "../src/player/randomization.h"

/**
 * PLAYER
 */
#define PLAYER_CHAR                     '@'

#define PLAYER_DEFAULT_INVENTORY_SIZE   20

/** Attributes */
#define PLAYER_DEFAULT_EYESIGHT         5


/**
 * KEYBINDINGS
 */
#define KEY_NORTH   'w'
#define KEY_EAST    'd'
#define KEY_SOUTH   's'
#define KEY_WEST    'a'

/**
 * MATERIAL
 */
extern Range MATERIAL_STRENGTH[];


/**
 * LEVEL
 */
#define LEVEL_ITEM_COUNT(level) \
    (uint16_t) ((level->size.height * level->size.width) / 50);


extern LevelConfig LEVEL_CAVE;


/**
 * CELL
 */
extern CellRegistryConfig CELL_REGISTRY_VOID;

extern CellRegistryConfig CELL_REGISTRY_CAVE;

extern CellRegistryConfig CELL_REGISTRY_WATER;


/**
 * ITEMS
 */
extern Probability ITEM_TYPE_PROBABILITY;

extern Probability ITEM_CONSUMABLE_PROBABILITY;

extern Probability ITEM_TOOL_PROBABILITY;


#endif
