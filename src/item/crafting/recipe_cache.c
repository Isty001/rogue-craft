#include "memory/memory.h"
#include "recipe_registry.h"
#include "util/environment.h"


#define cache_open_recipes(cache) \
    cache_open(cache, RESOURCE_RECIPES, sizeof(Recipe));



CacheError recipe_cache_load(void)
{
    return CE_NOT_FOUND;
}

void recipe_cache_save(Probability *recipes)
{
    //TODO recipe cache
    (void)recipes;
}
