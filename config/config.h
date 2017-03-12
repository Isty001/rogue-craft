#ifndef ROGUECRAFT_DEFAULTS_H
#define ROGUECRAFT_DEFAULTS_H


#include "../src/color.h"
#include "../src/level/cell.h"
#include "../src/level/level.h"
#include "../src/player/player.h"
#include "../src/player/item.h"
#include "../src/randomization.h"
#include "../src/event.h"


/**
 * DIRECTORIES
 */
#define DIR_CONFIG_ITEMS DIR_CONFIG"/items"

extern Listener LISTENERS[];

/**
 * PLAYER
 */
#define PLAYER_CHAR                     '@'

#define PLAYER_DEFAULT_INVENTORY_SIZE   20

/** Attributes */
#define PLAYER_DEFAULT_EYESIGHT         5

extern AttributeConfig PLAYER_ATTRIBUTE_CONFIG;


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
    (uint16_t) ((level->size.height * level->size.width) / 500);


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
