#include "unit_test.h"
#include "../src/json.h"

/**
 * color_add() starts from 100, to have space for custom pairs
 */
MU_TEST(test_style)
{
    JSON_Value *root = json_parse_file(DIR_FIXTURE"/style.json");
    JSON_Array *array = json_value_get_array(root);

    Style actual = json_get_style(json_array_get_object(array, 0));
    mu_assert_int_eq(0 | COLOR_PAIR(COLOR_PAIR_RED_F) | A_BOLD, actual);

    actual = json_get_style(json_array_get_object(array, 1));
    mu_assert_int_eq(COLOR_PAIR(100) | A_BOLD, actual);

    json_value_free(root);
}

MU_TEST(test_optional_style)
{
    JSON_Value *root = json_parse_string("{}");
    JSON_Object *obj = json_value_get_object(root);

    mu_assert_int_eq(13, json_get_optional_style(obj, 13));

    json_value_free(root);
}

void run_json_test(void)
{
    color_init();

    TEST_NAME("Json");

    MU_RUN_TEST(test_style);
    MU_RUN_TEST(test_optional_style);

    MU_REPORT();

    color_cleanup();
    unlink(DIR_CACHE"/"CACHE_COLORS".cache");
}
