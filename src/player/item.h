#ifndef ROGUECRAFT_ITEM_H
#define ROGUECRAFT_ITEM_H


#include "player.h"
#include "../ncurses/ncurses.h"


typedef enum {
    IE_CONSUMED,
    IE_REPEAT,
    IE_INVALID_ARGUMENT,
    IE_DUPLICATE,
    IE_OK,
    IE_OVERFLOW
} ItemError;

typedef enum {
    CONSUMABLE,
    TOOL
} ItemType;

typedef struct Consumable {
    bool permanent;
    AttributeType type;
} Consumable;

typedef struct {
    char *name;
    char *description;
    wchar_t chr;
    Style style;
    ItemType type;
    uint16_t value;
    union {
        Consumable consumable;
    };
} Item;

typedef const struct ItemPrototype {
    Item item;
    Range value_range;
    Item *(*randomize)(const struct ItemPrototype *);
} ItemPrototype;


void item_pool_init(void);

Item *item_clone(ItemPrototype *prototype);

ItemError item_consume(Item *item, Player *player);

Item *item_random(void);

void item_free(Item *item);

void item_pool_cleanup(void);


#endif
