#include <list.h>
#include "json.h"


static void walk_array(JSON_Array *array, JsonParser parse)
{
    size_t count = json_array_get_count(array);

    for (size_t i = 0; i < count; i++) {
        parse(json_array_get_object(array, i));
    }
}

void json_parse_in_dir(char *dir, JsonParser parser)
{
    JSON_Value *json;
    JSON_Array *array;

    dir_foreach(dir, function(void, (tinydir_file * file) {
        json = json_parse_file(file->path);

        if (json && (array = json_value_get_array(json))) {
            walk_array(array, parser);
            json_value_free(json);
        }
    }));
}

static ColorPair parse_new_color(JSON_Object *json)
{
    Color fore = (Color) json_get_optional_number(json, "fore", -1);
    Color back = (Color) json_get_optional_number(json, "back", -1);

    return color_add(fore, back);
}

static Style parse_style(JSON_Value *elem, JSON_Value_Type type)
{
    if (type == JSONObject) {
        return parse_new_color(json_value_get_object(elem));
    } else if (type == JSONString) {
        return constant(json_value_get_string(elem));
    }
    return 0;
}

Style json_get_style(JSON_Object *json)
{
    JSON_Array *definition = json_get_array(json, "style");
    size_t count = json_array_get_count(definition);
    JSON_Value_Type type;
    JSON_Value *elem;
    Style style = 0;

    for (size_t i = 0; i < count; i++) {
        elem = json_array_get_value(definition, i);
        type = json_value_get_type(elem);
        style |= parse_style(elem, type);
    }
    return style;
}

Style json_get_optional_style(JSON_Object *json, Style fallback)
{
    if (json_has_array(json, "style")) {
        return json_get_style(json);
    }
    return fallback;
}

void json_get_optional_wchar(wchar_t *buff, JSON_Object *json, char *name, wchar_t fallback)
{
    if (json_has_string(json, name)) {
        mbtowc(buff, json_object_get_string(json, name), 2);
    } else {
        *buff = fallback;
    }
}

uint64_t json_get_optional_constant(JSON_Object *json, char *name, uint64_t fallback)
{
    if (json_has_string(json, name)) {
        return constant(json_object_get_string(json, name));
    }
    return fallback;
}
