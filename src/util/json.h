#ifndef ROGUE_CRAFT_JSON_H
#define ROGUE_CRAFT_JSON_H


#include <parson.h>
#include "color.h"


typedef void (*JsonParser)(JSON_Object *object);


#define json_get(json, name, type, parson_type) ({                          \
    if (!json_object_has_value_of_type(json, name, parson_type)) {          \
        fatal("Missing value: [%s], with [%s] type", name, #type);          \
    }                                                                       \
    json_object_get_##type(json, name); })

#define json_get_optional(json, name, type, c_type, parson_type, default) ({    \
    c_type ret;                                                                 \
    if (!json_object_has_value_of_type(json, name, parson_type)) {              \
        ret = default;                                                          \
    } else { ret =  json_object_get_##type(json, name); }                       \
        ret;                                                                    \
    })


#define json_dot_get(json, path, type, parson_type) ({                      \
    if (!json_object_dothas_value_of_type(json, path, parson_type)) {       \
        fatal("Missing value: [%s], with [%s] type", path, #type);          \
    }                                                                       \
    json_object_dotget_##type(json, path); })

#define json_get_string(json, name) \
     (char *) json_get(json, name, string, JSONString)

#define json_get_wchar(buff, json, name) \
    mbtowc(buff, json_get_string(json, name), 2);

#define json_get_array(json, name) \
     json_get(json, name, array, JSONArray)

#define json_get_object(json, name) \
     json_get(json, name, object, JSONObject)

#define json_get_bool(json, name) \
     json_get(json, name, boolean, JSONBoolean)

#define json_get_optional_bool(json, name) \
     json_get_optional(json, name, boolean, bool, JSONBoolean, false)

#define json_get_number(json, name) \
    json_get(json, name, number, JSONNumber)

#define json_dot_get_number(json, path) \
    json_dot_get(json, path, number, JSONNumber)

#define json_has_object(json, name) \
    json_object_has_value_of_type(json, name, JSONObject)

#define json_has_string(json, name) \
    json_object_has_value_of_type(json, name, JSONString)

#define json_has_array(json, name) \
    json_object_has_value_of_type(json, name, JSONArray)

#define json_get_optional_number(json, name, default) \
    json_get_optional(json, name, number, double, JSONNumber, default)


void json_parse_in_dir(char *dir, JsonParser parser);

Style json_get_style(JSON_Object *json);

Style json_get_optional_style(JSON_Object *json, Style fallback);

void json_get_optional_wchar(wchar_t *buff, JSON_Object *json, char *name, wchar_t fallback);

uint64_t json_get_optional_constant(JSON_Object *json, char *name, char *prefix, uint64_t fallback);


#endif
