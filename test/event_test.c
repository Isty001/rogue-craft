#include "minunit.h"
#include "unit_test.h"
#include "../src/event.h"


#define TEST_EVENT_1 0
#define TEST_EVENT_2 2


#define mark(n, e) TEST_EVENT_##n##_INVOKED += *(int *)e->data


static int TEST_EVENT_1_INVOKED = 0;
static int TEST_EVENT_2_INVOKED = 0;


static EventError on_test_event_1(Event *event)
{
    mark(1, event);

    return EE_OK;
}

static EventError on_test_event_1_again(Event *event)
{
    mark(1, event);

    return EE_OK;
}

static EventError on_test_event_2(Event *event)
{
    mark(2, event);

    return EE_BREAK;
}

static EventError on_test_event_2_again(Event *event)
{
    mark(2, event);

    return EE_OK;
}

Listener LISTENERS[5] = {
    {.listening = TEST_EVENT_1, .handle = on_test_event_1},
    {.listening = TEST_EVENT_1, .handle = on_test_event_1_again},
    {.listening = TEST_EVENT_2, .handle = on_test_event_2},
    {.listening = TEST_EVENT_2, .handle = on_test_event_2_again},
    {.handle = NULL}
};

MU_TEST(test_event)
{
    event_register_listeners(LISTENERS);

    int data = 100;

    event_dispatch(&event_new(TEST_EVENT_1, &data));
    event_dispatch(&event_new(TEST_EVENT_2, &data));

    mu_assert_int_eq(200, TEST_EVENT_1_INVOKED);
    mu_assert_int_eq(100, TEST_EVENT_2_INVOKED);
}

void run_event_test(void)
{
    TEST_NAME("Event");

    MU_RUN_TEST(test_event);

    MU_REPORT();
}
