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
    MELEE
} ItemType;

typedef struct Consumable {
    uint16_t value;
    bool permanent;
    AttributeType type;
} Consumable;

typedef struct {
    char *name;
    char *description;
    char chr;
    Color color;
    ItemType type;
    union {
        Consumable consumable;
    };
} Item;

typedef const struct {
    Item data;
    void (*randomize)(Item *);
} ItemPrototype;

typedef const struct {
    uint16_t size;
    uint16_t all;
    struct {
        uint16_t value;
        ItemPrototype *item;
    } probabilities[];
} ItemProbability;



Item *item_clone(ItemPrototype *prototype);

ItemError item_consume(Item *item, Player *player);

void item_free(Item *item);


#endif
