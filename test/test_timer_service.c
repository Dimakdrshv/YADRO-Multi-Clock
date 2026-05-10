#include "unity.h"

#include "timer_service.h"
#include "apps/event/app_events.h"

#include "mock_timer_driver.h"

void setUp(void)
{

}

void tearDown(void) 
{

}

static void check_timer_irq(uint32_t value)
{
    TIMER_get_overflow_flag_ExpectAndReturn(value);

    uint32_t events = TIMER_Service_GetIRQEvents();

    if (value == 1) {
        TEST_ASSERT_BITS
        (
            APP_EVENT_TIMER32_1_IS_OVERFLOW_M,
            APP_EVENT_TIMER32_1_IS_OVERFLOW_M,
            events
        );
    } 
    else {
        TEST_ASSERT_BITS
        (
            APP_EVENT_NONE_M,
            APP_EVENT_NONE_M,
            events
        );
    }
}

void test_IRQ_timer_happend(void)
{
    check_timer_irq(1);
}

void test_IRQ_timer_not_happend(void)
{
    check_timer_irq(0);
}
