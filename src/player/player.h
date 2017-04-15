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
    struct {
        time_t fatigue;
        time_t fatigue_damage;
    } timestamp;
} Modifiers;

typedef struct {
    pthread_mutex_t mutex;
    Modifiers modifiers;
    Attribute attributes[PLAYER_ATTR_NUM];
} PlayerState;

typedef struct Player {
    Level *level;
    Camera *camera;
    Inventory *inventory;
    PlayerState state;
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
    uint16_t traveled;
    uint16_t dealt_damage;
    uint16_t elapsed_time;
    Range thirst;
    Range hunger;
    Range stamina;
} Fatigue;

typedef const struct {
    uint16_t hunger;
    uint16_t thirst;
    uint16_t elapsed_time;
    Range health;
} FatigueDamage;

typedef const struct {
    Fatigue fatigue;
    FatigueDamage fatigue_damage;
} PlayerStateConfig;


Player *player_new(Level *level, Camera *camera);

void player_free(Player *player);

EventError player_move(InputEvent *event);

void player_sight_update(Player *player);

bool player_can_see(Player *player, Cell *cell, Point point);

EventError player_hit(InteractionEvent *event);

void player_attributes_display(Player *player);

void player_position_on_level(Player *player);

void player_state_update(Player *player, PlayerStateConfig *cfg);


#endif
