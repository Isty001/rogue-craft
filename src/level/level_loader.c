#include "util/json.h"
#include "level.h"
#include "config.h"
#include "memory/memory.h"


Probability LEVEL_PROBABILITY;


static void build_cells(Probability *cells, JSON_Object *map)
{
    const Cell *cell;
    uint16_t chance;
    char *name;
    size_t count = json_object_get_count(map);

    for (size_t i = 0; i < count; i++) {
        name = (char *) json_object_get_name(map, i);
        chance = (uint16_t) json_get_number(map, name);
        cell = cell_registry_get(name);

        probability_add(cells, chance, (const void *) cell);
    }
}

static void add_level(LevelConfig *cfg, JSON_Object *json)
{
    uint16_t chance = (uint16_t) json_get_number(json, "probability");

    probability_add(&LEVEL_PROBABILITY, chance, cfg);
}

static void parse_json(JSON_Object *json)
{
    LevelConfig *cfg = mem_calloc(1, sizeof(LevelConfig));
    cfg->type = (LevelType) constant_prefixed("LEVEL", json_get_string(json, "type"));

    JSON_Object *cells = json_get_object(json, "cells");

    build_cells(&cfg->cells.hollow, json_get_object(cells, "HOLLOW"));
    build_cells(&cfg->cells.solid, json_get_object(cells, "SOLID"));

    add_level(cfg, json);
}

void level_load(void)
{
    if (CE_LOADED != level_cache_load()) {
        json_parse_in_dir(env_json_resource_path(RESOURCE_LEVELS), parse_json);
        level_cache_save();
    }
}

void level_unload(void)
{
    probability_clean(&LEVEL_PROBABILITY, mem_dealloc);
}
