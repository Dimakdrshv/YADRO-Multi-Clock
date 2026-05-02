#include "lcd_service.h"
#include "apps/event/app_events.h"
#include "drivers/lcd/lcd_driver.h"

static uint32_t events = APP_EVENT_NONE_M;

static void LCD_Service_Format2Digits(char *dst, uint8_t value) {
    dst[0] = (char)('0' + (value / 10u));
    dst[1] = (char)('0' + (value % 10u));
}

static void LCD_Service_Format4Digits(char *dst, uint16_t value) {
    dst[0] = (char)('0' + ((value / 1000u) % 10u));
    dst[1] = (char)('0' + ((value / 100u) % 10u));
    dst[2] = (char)('0' + ((value / 10u) % 10u));
    dst[3] = (char)('0' + (value % 10u));
}

static void LCD_Service_FormatTime(char line[17], uint8_t hours, uint8_t minutes, uint8_t seconds) {
    LCD_Service_Format2Digits(&line[0], hours);
    line[2] = ':';
    LCD_Service_Format2Digits(&line[3], minutes);
    line[5] = ':';
    LCD_Service_Format2Digits(&line[6], seconds);

    for (uint8_t i = 8; i < 16; i++) {
        line[i] = ' ';
    }
    line[16] = '\0';
}

static void LCD_Service_FormatDate(char line[17], uint8_t day, uint8_t month, uint16_t year) {
    LCD_Service_Format2Digits(&line[0], day);
    line[2] = '.';
    LCD_Service_Format2Digits(&line[3], month);
    line[5] = '.';
    LCD_Service_Format4Digits(&line[6], year);

    for (uint8_t i = 10; i < 16; i++) {
        line[i] = ' ';
    }
    line[16] = '\0';
}

static void LCD_Service_FormatAlarmTime(char line[17], uint8_t hours, uint8_t minutes) {
    line[0] = ' ';
    line[1] = ' ';
    line[2] = 'A';
    line[3] = 'L';
    line[4] = 'A';
    line[5] = 'R';
    line[6] = 'M';
    line[7] = ':';
    line[8] = ' ';
    LCD_Service_Format2Digits(&line[9], hours);
    line[11] = ':';
    LCD_Service_Format2Digits(&line[12], minutes);

    for (uint8_t i = 14; i < 16; i++) {
        line[i] = ' ';
    }
    line[16] = '\0';
}

void LCD_Service_Clear(void) {
    if (LCD_clear() != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_CLEAR_ERROR_M;
    }
}

void LCD_Service_ShowClock(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year) {
    char line1[17];
    char line2[17];
    LCD_Service_FormatTime(line1, hours, minutes, seconds);
    LCD_Service_FormatDate(line2, day, month, year);
    if (LCD_set_cursor(0u, 0u) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M;
        return;
    }
    if (LCD_write_string(line1) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_WRITE_ERROR_M;
        return;
    }
    if (LCD_set_cursor(1u, 0u) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M;
        return;
    }
    if (LCD_write_string(line2) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_WRITE_ERROR_M;
        return;
    }
}

void LCD_Service_ShowAlarm(uint8_t hours, uint8_t minutes) {
    char line[17];
    LCD_Service_FormatAlarmTime(line, hours, minutes);
    if (LCD_set_cursor(0u, 0u) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M;
        return;
    }
    if (LCD_write_string(line) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_WRITE_ERROR_M;
        return;
    }
}

static void LCD_Service_FormatTemperature(char line[17], uint8_t temperature) {
    line[0] = ' ';
    line[1] = ' ';
    line[2] = ' ';
    line[3] = 'T'; 
    line[4] = 'E';
    line[5] = 'M';
    line[6] = 'P';
    line[7] = ':';
    line[8] = ' ';
    LCD_Service_Format2Digits(&line[9], temperature);
    line[11] = '\xDF';
    line[12] = 'C';

    for (uint8_t i = 13; i < 16; i++) {
        line[i] = ' ';
    }
    line[16] = '\0';
}

static void LCD_Service_FormatHumidity(char line[17], uint8_t humidity) {
    line[0] = ' ';
    line[1] = ' ';
    line[2] = ' ';
    line[3] = ' ';
    line[4] = 'H'; 
    line[5] = 'U';
    line[6] = 'M';
    line[7] = ':';
    line[8] = ' ';
    LCD_Service_Format2Digits(&line[9], humidity);
    line[11] = '%';

    for (uint8_t i = 12; i < 16; i++) {
        line[i] = ' ';
    }
    line[16] = '\0';
}

void LCD_Service_ShowTemperatureHumidity(uint8_t temperature, uint8_t humidity) {
    char line1[17];
    char line2[17];
    LCD_Service_FormatTemperature(line1, temperature);
    LCD_Service_FormatHumidity(line2, humidity);
    if (LCD_set_cursor(0u, 0u) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M;
        return;
    }
    if (LCD_write_string(line1) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_WRITE_ERROR_M;
        return;
    }
    if (LCD_set_cursor(1u, 0u) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M;
        return;
    }
    if (LCD_write_string(line2) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_WRITE_ERROR_M;
        return;
    }
}

void LCD_Service_ShowStartup(void) {
    char line1[17] = "     YADRO     ";
    char line2[17] = "  MULTI CLOCK  ";
    if (LCD_set_cursor(0u, 0u) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M;
        return;
    }
    if (LCD_write_string(line1) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_WRITE_ERROR_M;
        return;
    }
    if (LCD_set_cursor(1u, 0u) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_SETCURSOR_ERROR_M;
        return;
    }
    if (LCD_write_string(line2) != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_WRITE_ERROR_M;
        return;
    }
}

void LCD_Service_DisplayOn(void) {
    if (LCD_display_on() != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_DISPLAY_ERROR_M;
    }
}

void LCD_Service_DisplayOff(void) {
    if (LCD_display_off() != LCD_DRIVER_OK) {
        events |= APP_EVENT_LCD_DRIVER_DISPLAY_ERROR_M;
    }
}

uint32_t LCD_Service_GetEvents(void) {
    uint32_t current_events = events;
    events = APP_EVENT_NONE_M;
    return current_events;
}