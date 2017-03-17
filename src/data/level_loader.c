#include "../json.h"
#include "../../config/config.h"


Probability LEVEL_PROBABILITY;


static void build_cells(Probability *cells, JSON_Array *array)
{
    const Cell *cell;
    JSON_Object *cell_json;
    uint16_t chance;
    char *name;
    size_t count = json_array_get_count(array);

    for (size_t i = 0; i < count; i++) {
        cell_json = json_array_get_object(array, i);
        name = (char *) json_object_get_name(cell_json, 0);
        cell = cell_get(name);
        chance = (uint16_t) json_get_number(cell_json, name);

        probability_add(cells, chance, (Randomizable) cell);
    }
}

static void add_level(LevelConfig *cfg, JSON_Object *json)
{
    uint16_t chance = (uint16_t) json_get_number(json, "probability");

    probability_add(&LEVEL_PROBABILITY, chance, cfg);
}

static void parse_json(JSON_Object *json)
{
    LevelConfig *cfg = callocate(1, sizeof(LevelConfig));
    cfg->type = (LevelType) constant(json_get_string(json, "type"));

    JSON_Object *cells = json_get_object(json, "cells");

    build_cells(&cfg->cells.hollow, json_get_array(cells, "HOLLOW"));
    build_cells(&cfg->cells.solid, json_get_array(cells, "SOLID"));

    add_level(cfg, json);
}

void level_load(void)
{
    if (CE_LOADED != level_cache_load()) {
        json_parse_in_dir(DIR_CONFIG_LEVEL, parse_json);
        level_cache_save();
    }
}

void level_unload(void)
{
    probability_clean(&LEVEL_PROBABILITY, release);
}
