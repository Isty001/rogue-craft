#ifndef ROGUECRAFT_ITEM_H
#define ROGUECRAFT_ITEM_H


#include "player.h"
#include "../ncurses/ncurses.h"
#include "../event.h"
#include "../storage/storage.h"


#define ITEM_NAME_MAX 20


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
    AttributeType attribute;
} Consumable;

typedef struct {
    Style style;
} LightSource;

typedef struct Item {
    char name[ITEM_NAME_MAX];
    wchar_t chr;
    Style style;
    int16_t value;
    ItemType type;
    union {
        Consumable consumable;
        Tool tool;
        LightSource light_source;
    };
} Item;

typedef struct ItemPrototype {
    Range value_range;
    Item item;
} ItemPrototype;


void item_load(void);

CacheError item_cache_load(void);

void item_cache_save(void);

void item_unload(void);

void item_pool_init(void);

Item *item_clone(ItemPrototype *prototype);

Item *item_random(void);

ItemError item_consume(Item *item, Player *player);

EventError item_pickup(InteractionEvent *event);

Item *item_allocate(void);

void item_free(Item *item);

void item_pool_cleanup(void);


#endif
