#ifndef ROGUECRAFT_ITEM_H
#define ROGUECRAFT_ITEM_H


#include "player.h"


typedef enum {
    IE_CONSUMED,
    IE_REPEAT,
    IE_INVALID_ARGUMENT
} ItemError;

typedef struct Consumable {
    int value;
    bool persistent;
    AttributeType type;
} Consumable;

typedef enum {
    CONSUMABLE,
    MELEE
} ItemType;

typedef struct {
    char name[10];
    char description[30];
    char chr;
    Color color;
    ItemType type;
    union {
        Consumable consumable;
    };
} Item;


ItemError item_consume(Item *item, Player *player);


#endif
