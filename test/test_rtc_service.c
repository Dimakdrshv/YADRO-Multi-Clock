#include "unity.h"

#include "rtc_service.h"
#include "apps/event/app_events.h"

#include "mock_rtc_driver.h"

#define DEFAULT_DOW_TEST      (1u)
#define DEFAULT_SECONDS_TEST  (0u)

void setUp(void)
{
    RTC_Service_GetEvents();
}

void tearDown(void)
{

}

void test_RTC_SETTIME_funtion(void)
{
    RTC_Time expected_time = {
        .hours = 12,
        .minutes = 30,
        .seconds = 45,
        .dow = DEFAULT_DOW_TEST
    };

    RTC_set_time_ExpectWithArrayAndReturn(
        &expected_time,
        1,
        RTC_DRIVER_OK
    );

    RTC_Service_SetTime(12, 30, 45);

    TEST_ASSERT_BITS(
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_SETDATE_funtion(void)
{
    RTC_Date expected_date = {
        .day = 10,
        .month = 5,
        .year = 26,
        .century = 20
    };

    RTC_set_date_ExpectWithArrayAndReturn(
        &expected_date,
        1,
        RTC_DRIVER_OK
    );

    RTC_Service_SetDate(10, 5, 26, 20);

    TEST_ASSERT_BITS(
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_SETALARMTIME_funtion(void)
{
    RTC_Time expected_time = {
        .hours = 7,
        .minutes = 30,
        .seconds = DEFAULT_SECONDS_TEST,
        .dow = DEFAULT_DOW_TEST
    };

    RTC_set_alarm_time_ExpectWithArrayAndReturn(
        &expected_time,
        1,
        RTC_DRIVER_OK
    );

    RTC_Service_SetAlarmTime(7, 30);

    TEST_ASSERT_BITS(
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_GETTIME_funtion(void)
{
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    RTC_Time driver_time = {
        .hours = 12,
        .minutes = 30,
        .seconds = 45,
        .dow = DEFAULT_DOW_TEST
    };

    RTC_get_time_ExpectAndReturn(NULL, RTC_DRIVER_OK);
    RTC_get_time_IgnoreArg_time();
    RTC_get_time_ReturnMemThruPtr_time(&driver_time, sizeof(driver_time));

    RTC_Service_GetTime(&hours, &minutes, &seconds);

    TEST_ASSERT_EQUAL_UINT8(12, hours);
    TEST_ASSERT_EQUAL_UINT8(30, minutes);
    TEST_ASSERT_EQUAL_UINT8(45, seconds);

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_GETDATE_funtion(void)
{
    uint8_t day = 0;
    uint8_t month = 0;
    uint8_t year = 0;
    uint8_t century = 0;

    RTC_Date driver_date = {
        .day = 10,
        .month = 5,
        .year = 26,
        .century = 20
    };

    RTC_get_date_ExpectAndReturn(NULL, RTC_DRIVER_OK);
    RTC_get_date_IgnoreArg_date();
    RTC_get_date_ReturnMemThruPtr_date(&driver_date, sizeof(driver_date));

    RTC_Service_GetDate(&day, &month, &year, &century);

    TEST_ASSERT_EQUAL_UINT8(10, day);
    TEST_ASSERT_EQUAL_UINT8(5, month);
    TEST_ASSERT_EQUAL_UINT8(26, year);
    TEST_ASSERT_EQUAL_UINT8(20, century);

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_GETALARMTIME_funtion(void)
{
    uint8_t hours = 0;
    uint8_t minutes = 0;

    RTC_Time driver_time = {
        .hours = 7,
        .minutes = 30,
        .seconds = DEFAULT_SECONDS_TEST,
        .dow = DEFAULT_DOW_TEST
    };

    RTC_get_alarm_time_ExpectAndReturn(NULL, RTC_DRIVER_OK);
    RTC_get_alarm_time_IgnoreArg_time();
    RTC_get_alarm_time_ReturnMemThruPtr_time(&driver_time, sizeof(driver_time));

    RTC_Service_GetAlarmTime(&hours, &minutes);

    TEST_ASSERT_EQUAL_UINT8(7, hours);
    TEST_ASSERT_EQUAL_UINT8(30, minutes);

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_ENABLEALARM_funtion(void)
{
    RTC_alarm_enable_Expect();

    RTC_Service_EnableAlarm();
}

void test_RTC_DISABLEALARM_funtion(void)
{
    RTC_alarm_disable_Expect();

    RTC_Service_DisableAlarm();
}

void test_RTC_DRIVER_SETTIME_event(void)
{
    RTC_set_time_ExpectAndReturn(NULL, RTC_DRIVER_ERROR);
    RTC_set_time_IgnoreArg_time();

    RTC_Service_SetTime(12, 30, 45);

    TEST_ASSERT_BITS
    (
        APP_EVENT_RTC_DRIVER_SETTIME_ERROR_M,
        APP_EVENT_RTC_DRIVER_SETTIME_ERROR_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_DRIVER_SETDATE_event(void)
{
    RTC_set_date_ExpectAndReturn(NULL, RTC_DRIVER_ERROR);
    RTC_set_date_IgnoreArg_date();

    RTC_Service_SetDate(10, 5, 26, 20);

    TEST_ASSERT_BITS
    (
        APP_EVENT_RTC_DRIVER_SETDATE_ERROR_M,
        APP_EVENT_RTC_DRIVER_SETDATE_ERROR_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_DRIVER_SETALARM_event(void)
{
    RTC_set_alarm_time_ExpectAndReturn(NULL, RTC_DRIVER_ERROR);
    RTC_set_alarm_time_IgnoreArg_time();

    RTC_Service_SetAlarmTime(7, 30);

    TEST_ASSERT_BITS
    (
        APP_EVENT_RTC_DRIVER_SETALARM_ERROR_M,
        APP_EVENT_RTC_DRIVER_SETALARM_ERROR_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_DRIVER_GETTIME_event(void)
{
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    RTC_get_time_ExpectAndReturn(NULL, RTC_DRIVER_ERROR);
    RTC_get_time_IgnoreArg_time();

    RTC_Service_GetTime(&hours, &minutes, &seconds);

    TEST_ASSERT_BITS
    (
        APP_EVENT_RTC_DRIVER_GETTIME_ERROR_M,
        APP_EVENT_RTC_DRIVER_GETTIME_ERROR_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_DRIVER_GETDATE_event(void)
{
    uint8_t day = 0;
    uint8_t month = 0;
    uint8_t year = 0;
    uint8_t century = 0;

    RTC_get_date_ExpectAndReturn(NULL, RTC_DRIVER_ERROR);
    RTC_get_date_IgnoreArg_date();

    RTC_Service_GetDate(&day, &month, &year, &century);

    TEST_ASSERT_BITS
    (
        APP_EVENT_RTC_DRIVER_GETDATE_ERROR_M,
        APP_EVENT_RTC_DRIVER_GETDATE_ERROR_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_DRIVER_GETALARM_event(void)
{
    uint8_t hours = 0;
    uint8_t minutes = 0;

    RTC_get_alarm_time_ExpectAndReturn(NULL, RTC_DRIVER_ERROR);
    RTC_get_alarm_time_IgnoreArg_time();

    RTC_Service_GetAlarmTime(&hours, &minutes);

    TEST_ASSERT_BITS
    (
        APP_EVENT_RTC_DRIVER_GETALARM_ERROR_M,
        APP_EVENT_RTC_DRIVER_GETALARM_ERROR_M,
        RTC_Service_GetEvents()
    );
}

void test_RTC_ISALARM_event(void)
{
    RTC_get_alarm_flag_ExpectAndReturn(true);

    uint32_t events = RTC_Service_GetIRQEvents();

    TEST_ASSERT_BITS
    (
        APP_EVENT_RTC_IS_ALARM_M,
        APP_EVENT_RTC_IS_ALARM_M,
        events
    );
}

void test_RTC_NOALARM_event(void)
{
    RTC_get_alarm_flag_ExpectAndReturn(false);

    uint32_t events = RTC_Service_GetIRQEvents();

    
    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        events
    );
}