#include "item/crafting/crafting.h"
#include "../../unit_test.h"


MU_TEST(test_craft_requirements)
{
    CraftRequirements req = craft_collect_requiements();
}

void run_craft_test(void)
{
    TEST_NAME("Craft");

    MU_RUN_TEST(test_craft_requirements);

    MU_REPORT();
}
