#ifndef ROGUE_CRAFT_LIGHTING_H
#define ROGUE_CRAFT_LIGHTING_H


#include "../sight.h"


typedef struct Lighting Lighting;


size_t lighted_cell_size(void);

void lighted_cell_pool_init(void);

void lighted_cell_pool_cleanup(void);

Lighting *lighting_new(Level *level, Point source, uint16_t radius, Style style);

bool lighting_has(Lighting *lighting, Point point);

void lighting_update(Lighting *lighting, Point source, uint16_t radius);

void lighting_detach(Lighting *lighting);

void lighting_free(Lighting *lighting);


#endif
