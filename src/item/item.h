#ifndef ROGUECRAFT_ITEM_H
#define ROGUECRAFT_ITEM_H


#include "player/player.h"
#include "ui/ncurses.h"
#include "util/event.h"
#include "storage/storage.h"
#include "level/lighting/lighting.h"


#define ITEM_NAME_MAX 20

#define item_value(item) abs(item->value)


typedef enum {
    IE_CONSUMED,
    IE_CAN_BE_CONSUMED,
    IE_INVALID_ARGUMENT,
    IE_DUPLICATE,
    IE_OK,
    IE_OVERFLOW
} ItemError;

typedef enum {
    CONSUMABLE,
    TOOL,
    LIGHT_SOURCE,
    MATERIAL
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
    StateType state_type;
} Consumable;

typedef struct {
    uint16_t radius;
    Style style;
    bool portable;
    Lighting *lighting;
} LightSource;

typedef struct Item {
    const char *name;
    wchar_t chr;
    Style style;
    int16_t value;
    ItemType type;
    Cell *occupied_cell;
    union {
        Consumable consumable;
        Tool tool;
        LightSource light_source;
        Material material;
    };
} Item;

typedef struct ItemPrototype {
    char name[ITEM_NAME_MAX];
    Range value_range;
    Item item;
} ItemPrototype;


void item_pool_init(void);

Item *item_clone(const ItemPrototype *prototype);

ItemError item_consume(Item *item, Player *player);

void item_pickup(LevelInteractionEvent *event);

void item_light_source_place(LevelInteractionEvent *event);

Item *item_allocate(void);

void item_free(Item *item);

void item_pool_cleanup(void);


#endif
