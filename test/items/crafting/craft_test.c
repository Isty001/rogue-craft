#include "item/crafting/crafting.h"
#include "../../unit_test.h"


static const Recipe RECIPE = {

};


MU_TEST(test_successful_item_craft)
{

}

void run_craft_test(void)
{
    TEST_NAME("Craft");

    MU_RUN_TEST(test_successful_item_craft);

    MU_REPORT();
}
