#ifndef ROGUECRAFT_INVENTORY_H
#define ROGUECRAFT_INVENTORY_H


#include <stdint.h>
#include <stdbool.h>
#include "../color.h"
#include "player.h"


typedef struct item Item;

typedef struct consumable_item ConsumableItem;

struct consumable_item {
    int value;
    void (*consume)(ConsumableItem *, Player *);
};

typedef enum {
    CONSUMABLE
} ItemType;

struct item {
    char *name;
    char *description;
    char chr;
    Color color;
    ItemType type;
    union {
        ConsumableItem consumable;
    };
};

typedef struct {
    uint32_t size;
    Item *items;
} Inventory;


#endif
