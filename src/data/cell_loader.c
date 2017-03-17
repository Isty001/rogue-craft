#include <list.h>
#include "../../config/config.h"
#include "../json.h"


static List *PROTOTYPES;


static void extend(Cell *cell, JSON_Object *json, const Cell *parent)
{
    json_get_optional_wchar(&cell->chr, json, "char", parent->chr);
    cell->style = json_get_optional_style(json, parent->style);
    cell->type = (CellType) json_get_optional_constant(json, "type", parent->type);
    cell->material = (Material) json_get_optional_constant(json, "material", parent->material);
}

static void build_new(Cell *cell, JSON_Object *json)
{
    json_get_wchar(&cell->chr, json, "char");
    cell->style = json_get_style(json);
    cell->type = (CellType) constant(json_get_string(json, "type"));
    cell->material = (Material) constant(json_get_string(json, "material"));
}

static void finalize(JSON_Object *json, NamedCell *prototype, Cell *cell)
{
    cell->in_registry = true;
    cell->state = MATERIAL_STRENGTH[cell->material];
    prototype->cell = *cell;

    char *name = json_get_string(json, "name");
    sprintf(prototype->name, "%s", name);
}

static void build_cell(JSON_Object *json)
{
    Cell building;
    const Cell *parent;
    memset(&building, 0, sizeof(Cell));

    NamedCell *with_name = callocate(1, sizeof(NamedCell));

    if (json_has_string(json, "extends")) {
        parent = cell_get(json_get_string(json, "extends"));
        extend(&building, json, parent);
    } else {
        build_new(&building, json);
    }
    finalize(json, with_name, &building);

    PROTOTYPES->append(PROTOTYPES, with_name);
}

const Cell *cell_get(char *name)
{
    NamedCell *found = PROTOTYPES->find(PROTOTYPES, (Predicate) function(bool, (NamedCell * cell) {
        return 0 == strcmp(name, cell->name);
    }));

    if (!found) {
        fatal("Cant find Cell [%s]", name);
    }

    return &found->cell;
}

void cell_load(void)
{
    PROTOTYPES = list_new();
    PROTOTYPES->release_item = release;

    if (CE_LOADED != cell_cache_load(PROTOTYPES)) {
        json_parse_in_dir(DIR_CONFIG_CELLS, build_cell);
        cell_cache_save(PROTOTYPES);
    }
}

void cell_unload(void)
{
    PROTOTYPES->free(PROTOTYPES);
}
