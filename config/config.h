#ifndef ROGUECRAFT_DEFAULTS_H
#define ROGUECRAFT_DEFAULTS_H


#include "../src/color.h"
#include "../src/level/cell.h"
#include "../src/level/level.h"
#include "../src/player/player.h"
#include "../src/player/item.h"
#include "../src/player/randomization.h"

/**
 * USER
 */
#define PLAYER_CHAR                     '@'

#define PLAYER_DEFAULT_INVENTORY_SIZE   20

/** Attributes */
#define PLAYER_DEFAULT_EYESIGHT         4
#define PLAYER_DEFAULT_HP               100
#define PLAYER_DEFAULT_STAMINA          100
#define PLAYER_DEFAULT_HUNGER           0
#define PLAYER_LIMIT_HUNGER             100
#define PLAYER_DEFAULT_THIRST           0
#define PLAYER_LIMIT_THIRST             100


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
extern Material MATERIALS[];


/**
 * LEVEL
 */
#define LEVEL_ITEM_COUNT(level) \
    (uint16_t) ((level->size.height * level->size.width) / 2000);


extern LevelConfig LEVEL_CAVE;


/**
 * CELL
 */
extern CellRegistryConfig CELL_REGISTRY_CAVE;

extern CellRegistryConfig CELL_REGISTRY_VOID;


/**
 * ITEMS
 */
extern Probability ITEM_TYPE_PROBABILITY;

extern Probability ITEM_CONSUMABLE_PROBABILITY;

/**
 * ITEMS - CONSUMABLES
 */
extern ItemPrototype ITEM_CONSUMABLE_HP;

extern ItemPrototype ITEM_CONSUMABLE_MAX_HP;

extern ItemPrototype ITEM_CONSUMABLE_MAX_STAMINA;

extern ItemPrototype ITEM_CONSUMABLE_FOOD;

extern ItemPrototype ITEM_CONSUMABLE_WATER;


#endif
