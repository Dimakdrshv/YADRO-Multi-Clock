#include "unity.h"

#include "buzzer_service.h"
#include "apps/event/app_events.h"

#include "mock_buzzer_driver.h"

void setUp(void) 
{
    BUZZER_Service_GetEvents();
}

void tearDown(void) 
{

}

void test_SET_HIGH_function(void)
{
    BUZZER_set_value_Expect(true);

    BUZZER_Service_SetHigh();

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        BUZZER_Service_GetEvents()
    );
}

void test_SET_LOW_function(void)
{
    BUZZER_set_value_Expect(false);

    BUZZER_Service_SetLow();

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        BUZZER_Service_GetEvents()
    );
}