#ifndef ROGUE_CRAFT_SFX_H
#define ROGUE_CRAFT_SFX_H


typedef struct Cell Cell;


void sfx_init(void);

void sfx_play(const char *type, const char *file);

void sfx_play_rand(const char *type, const char *name);

void sfx_play_hit(Cell *cell);

void sfx_play_break(Cell *cell);

void sfx_play_step(Cell *cell);

void sfx_cleanup(void);


#endif
