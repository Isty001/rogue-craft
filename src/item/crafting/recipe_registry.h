#ifndef ROGUE_CRAFT_RECIPE_REGISTRY_H
#define ROGUE_CRAFT_RECIPE_REGISTRY_H


#include "util/randomization.h"
#include "storage/cache.h"
#include "recipe.h"


#define RESOURCE_RECIPES "recipes"


void recipe_registry_load(void);

CacheError recipe_cache_load(void);

void recipe_cache_save(Probability *recipes);

void recipe_registry_add(const Recipe *recipe, uint16_t chance);

const Probability *recipe_registry_all(void);

const Recipe *recipe_registry_random(void);

void recipe_registry_unload(void);


#endif
