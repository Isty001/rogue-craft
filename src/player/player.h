#ifndef ROGUECRAFT_PLAYER_H
#define ROGUECRAFT_PLAYER_H


#include <stdint.h>
#include <memory.h>
#include "util/timer.h"
#include "level/point.h"
#include "level/level.h"
#include "util/sight.h"
#include "util/event.h"


#define PLAYER_STATE_NUM      4
#define PLAYER_STATE_NAME_MAX 10
#define PLAYER_SPEED_MAX      150


typedef struct Inventory Inventory;

typedef struct Timer Timer;


typedef enum {
    HEALTH,
    STAMINA,
    HUNGER,
    THIRST
} StateType;

typedef struct {
    char name[PLAYER_STATE_NAME_MAX];
    Style style;
    uint16_t max;
    uint16_t current;
} State;

typedef struct {
    double dealt_damage;
    uint32_t traveled;
    struct {
        time_t fatigue;
        time_t fatigue_damage;
    } timestamp;
} Modifiers;

typedef struct {
    Direction direction;
    bool moving;
    Timer *timer;
} PlayerMovement;

typedef struct {
    Modifiers modifiers;
    State map[PLAYER_STATE_NUM];
} PlayerState;

typedef struct {
    uint16_t speed;
} PlayerAttributes;

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
    PlayerMovement movement;
    PlayerAttributes attributes;
    struct {
        Cell prototype;
        Cell *occupied;
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

void player_init_movement(Player *player);

void player_free(Player *player);

EventError player_move(InputEvent *event);

void player_sight_update(Player *player);

EventError player_hit(InteractionEvent *event);

void player_state_display(Player *player);

void player_position_on_level(Player *player);

void player_state_update(TimerArgs *args);


#endif
