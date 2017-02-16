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
    TOOL,
} ItemType;

typedef struct {
    double solid;
    double creature;
} DefaultDamage;

typedef struct {
    uint16_t range;
    struct {
        DefaultDamage defaults;
        double materials[MATERIAL_NUM];
    } multipliers;
} Tool;

typedef struct Consumable {
    bool permanent;
    AttributeType type;
} Consumable;

typedef struct Item {
    char *name;
    wchar_t chr;
    Style style;
    int16_t value;
    ItemType type;
    union {
        Consumable consumable;
        Tool tool;
    };
} Item;

typedef const struct ItemPrototype {
    Item item;
    Range value_range;
    Item *(*randomize)(const struct ItemPrototype *);
} ItemPrototype;


void item_pool_init(void);

Item *item_clone(ItemPrototype *prototype);

Item *item_random(void);

ItemError item_consume(Item *item, Player *player);

void item_free(Item *item);

void item_pool_cleanup(void);


#endif
