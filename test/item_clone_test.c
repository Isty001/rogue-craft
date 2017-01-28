#include "unit_test.h"
#include "../src/player/item.h"


#define create_prototype(randomizer) \
    (ItemPrototype) {.randomize = randomizer};


MU_TEST(test_no_randomize)
{
    ItemPrototype prototype = create_prototype(NULL);

    Item *item = item_clone(&prototype);
    item_free(item);
}

static void item_randomize(Item *item)
{
    item->consumable.value = 108;
}

MU_TEST(test_randomize)
{
    ItemPrototype prototype = create_prototype(item_randomize);
    Item *item = item_clone(&prototype);

    mu_assert_int_eq(108, item->consumable.value);

    item_free(item);
}

void run_item_clone_test(void)
{
    TEST_NAME("Item Clone");

    MU_RUN_TEST(test_no_randomize);
    MU_RUN_TEST(test_randomize);

    MU_REPORT();
}
