#ifndef ROGUECRAFT_PLAYER_H
#define ROGUECRAFT_PLAYER_H


#include <stdint.h>
#include <memory.h>
#include "../level/point.h"
#include "../level/level.h"
#include "../sight.h"
#include "../event.h"


#define PLAYER_ATTR_NUM      4
#define PLAYER_ATTR_NAME_MAX 10


typedef struct Inventory Inventory;


typedef enum {
    HEALTH,
    STAMINA,
    HUNGER,
    THIRST
} AttributeType;

typedef struct {
    char name[PLAYER_ATTR_NAME_MAX];
    Style style;
    uint16_t max;
    uint16_t current;
} Attribute;

typedef struct {
    double dealt_damage;
    uint32_t traveled;
    time_t updated;
} Modifiers;

typedef struct {
    pthread_mutex_t mutex;
    Modifiers modifiers;
    Attribute state[PLAYER_ATTR_NUM];
} PlayerAttributes;

typedef struct Player {
    Level *level;
    Camera *camera;
    Inventory *inventory;
    PlayerAttributes attributes;
    Sight *sight;
    struct {
        Point current;
        Point previous;
    } position;
    struct {
        Cell prototype;
        Cell *previous;
    } cell;
} Player;

typedef const struct {
    uint16_t time;
    uint16_t traveled;
    uint16_t dealt_damage;
    Range thirst;
    Range hunger;
    Range stamina;
} Fatigue;

typedef const struct {
    uint16_t hunger;
    uint16_t thirst;
    uint16_t time;
    Range health;
    Range stamina;
} FatigueDamage;

typedef const struct {
    Fatigue fatigue;
    FatigueDamage damage;
} AttributeConfig;


Player *player_new(Level *level, Camera *camera);

void player_free(Player *player);

EventError player_move(InputEvent *event);

void player_sight_update(Player *player);

bool player_can_see(Player *player, Point point);

EventError player_hit(InteractionEvent *event);

void player_attributes_display(Player *player);

void player_position_on_level(Player *player);


#endif
