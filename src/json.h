#ifndef ROGUE_CRAFT_JSON_H
#define ROGUE_CRAFT_JSON_H


#include <parson.h>


#define get(json, name, type, parson_type) ({                               \
    if (!json_object_has_value_of_type(json, name, parson_type)) {          \
        fatal("Missing value: [%s], with [%s] type", name, #type);          \
    }                                                                       \
    json_object_get_##type(json, name); })

#define dot_get(json, path, type, parson_type) ({                               \
    if (!json_object_dothas_value_of_type(json, path, parson_type)) {          \
        fatal("Missing value: [%s], with [%s] type", path, #type);          \
    }                                                                       \
    json_object_dotget_##type(json, path); })

#define get_string(json, name) \
     (char *) get(json, name, string, JSONString)

#define get_array(json, name) \
     get(json, name, array, JSONArray)

#define get_object(json, name) \
     get(json, name, object, JSONObject)

#define get_bool(json, name) \
     get(json, name, boolean, JSONBoolean)

#define get_number(json, name) \
    get(json, name, number, JSONNumber)

#define dot_get_number(json, path) \
    dot_get(json, path, number, JSONNumber)

#define has_object(json, name) \
    json_object_has_value_of_type(json, name, JSONObject)


#endif
