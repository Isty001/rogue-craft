#include <util/sight.h>
#include "util/memory.h"
#include "lighting.h"


Lighting *lighting_new(Level *level, Point source, uint16_t radius, Style style)
{
    Lighting *lighting = mem_alloc(sizeof(Lighting));
    lighting->sight = sight_new(level, source, radius);
    level->lightings->append(level->lightings, lighting);
    lighting->style = style;

    return lighting;
}

void lighting_free(Lighting *lighting)
{
    sight_free(lighting->sight);
    mem_dealloc(lighting);
}
