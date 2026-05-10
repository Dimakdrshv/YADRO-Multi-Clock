#include "unity.h"

#include "lcd_service.h"
#include "apps/event/app_events.h"

#include "mock_lcd_driver.h"

void setUp(void)
{
    LCD_Service_GetEvents();
}

void tearDown(void) 
{

}

void test_LCD_CLEAR_function(void)
{
    LCD_clear_ExpectAndReturn(LCD_DRIVER_OK);

    LCD_Service_Clear();

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_DISPLAYON_function(void)
{
    LCD_display_on_ExpectAndReturn(LCD_DRIVER_OK);

    LCD_Service_DisplayOn();

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_DISPLAYOFF_function(void)
{
    LCD_display_off_ExpectAndReturn(LCD_DRIVER_OK);

    LCD_Service_DisplayOff();

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_SHOWCLOCK_function(void)
{
    LCD_set_cursor_ExpectAndReturn(0, 0, LCD_DRIVER_OK);
    LCD_write_string_ExpectAndReturn("12:30:45        ", LCD_DRIVER_OK);

    LCD_set_cursor_ExpectAndReturn(1, 0, LCD_DRIVER_OK);
    LCD_write_string_ExpectAndReturn("10.05.2026      ", LCD_DRIVER_OK);

    LCD_Service_ShowClock(12, 30, 45, 10, 5, 2026);

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_SHOWALARM_function(void)
{
    LCD_set_cursor_ExpectAndReturn(0, 0, LCD_DRIVER_OK);
    LCD_write_string_ExpectAndReturn("  ALARM: 12:30  ", LCD_DRIVER_OK);

    LCD_Service_ShowAlarm(12, 30);

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_SHOWTEMPERATUREHUMIDITY_function(void)
{
    LCD_set_cursor_ExpectAndReturn(0, 0, LCD_DRIVER_OK);
    LCD_write_string_ExpectAndReturn("   TEMP: 23" "\xDF" "C   ", LCD_DRIVER_OK);

    LCD_set_cursor_ExpectAndReturn(1, 0, LCD_DRIVER_OK);
    LCD_write_string_ExpectAndReturn("    HUM: 55%    ", LCD_DRIVER_OK);

    LCD_Service_ShowTemperatureHumidity(23, 55);

    TEST_ASSERT_BITS(
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_SHOWSTARTUP_function(void)
{
    LCD_set_cursor_ExpectAndReturn(0, 0, LCD_DRIVER_OK);
    LCD_write_string_ExpectAndReturn("     YADRO     ", LCD_DRIVER_OK);

    LCD_set_cursor_ExpectAndReturn(1, 0, LCD_DRIVER_OK);
    LCD_write_string_ExpectAndReturn("  MULTI CLOCK  ", LCD_DRIVER_OK);

    LCD_Service_ShowStartup();

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_CLEAR_event(void)
{
    LCD_clear_ExpectAndReturn(LCD_DRIVER_ERROR);

    LCD_Service_Clear();

    TEST_ASSERT_BITS
    (
        APP_EVENT_LCD_DRIVER_CLEAR_ERROR_M,
        APP_EVENT_LCD_DRIVER_CLEAR_ERROR_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_SETCURSOR_event(void)
{
    LCD_set_cursor_ExpectAndReturn(0, 0, LCD_DRIVER_ERROR);

    LCD_Service_ShowClock(12, 30, 45, 10, 5, 2026);

    TEST_ASSERT_BITS(
        APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M,
        APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_WRITE_event(void)
{
    LCD_set_cursor_ExpectAndReturn(0, 0, LCD_DRIVER_OK);
    LCD_write_string_ExpectAndReturn("12:30:45        ", LCD_DRIVER_ERROR);

    LCD_Service_ShowClock(12, 30, 45, 10, 5, 2026);

    TEST_ASSERT_BITS(
        APP_EVENT_LCD_DRIVER_WRITE_ERROR_M,
        APP_EVENT_LCD_DRIVER_WRITE_ERROR_M,
        LCD_Service_GetEvents()
    );
}

void test_LCD_DISPLAY_event(void)
{
    LCD_display_on_ExpectAndReturn(LCD_DRIVER_ERROR);

    LCD_Service_DisplayOn();

    TEST_ASSERT_BITS
    (
        APP_EVENT_LCD_DRIVER_DISPLAY_ERROR_M,
        APP_EVENT_LCD_DRIVER_DISPLAY_ERROR_M,
        LCD_Service_GetEvents()
    );
}