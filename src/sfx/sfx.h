#ifndef ROGUE_CRAFT_SFX_H
#define ROGUE_CRAFT_SFX_H


typedef struct Cell Cell;


void sfx_init(void);

void sfx_play(char *type, char *file);

void sfx_play_rand(char *type, char *name);

void sfx_play_hit(Cell *cell);

void sfx_play_break(Cell *cell);

void sfx_play_step(Cell *cell);

void sfx_cleanup(void);


#endif
