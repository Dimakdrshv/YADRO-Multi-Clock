#include "rtc_service.h"
#include "apps/event/app_events.h"
#include "drivers/rtc/rtc_driver.h"

#define DEFAULT_DOW     (1u)
#define DEFAULT_SECONDS (0u)

static uint32_t events = APP_EVENT_NONE_M;

void RTC_Service_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    RTC_Time time = { .hours = hours, .minutes = minutes, .seconds = seconds, .dow = DEFAULT_DOW };
    
    if (RTC_set_time(&time) != RTC_DRIVER_OK) {
        events |= APP_EVENT_RTC_DRIVER_SETTIME_ERROR_M;
    }
}

void RTC_Service_SetDate(uint8_t day, uint8_t month, uint8_t year, uint8_t century) {
    RTC_Date date = { .day = day, .month = month, .year = year, .century = century };

    if (RTC_set_date(&date) != RTC_DRIVER_OK) {
        events |= APP_EVENT_RTC_DRIVER_SETDATE_ERROR_M;
    }
}

void RTC_Service_SetAlarmTime(uint8_t hours, uint8_t minutes) {
    RTC_Time time = { .hours = hours, .minutes = minutes, .seconds = DEFAULT_SECONDS, .dow = DEFAULT_DOW };

    if (RTC_set_alarm_time(&time) != RTC_DRIVER_OK) {
        events |= APP_EVENT_RTC_DRIVER_SETALARM_ERROR_M;
    }
}

void RTC_Service_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds) {
    RTC_Time time = {0};

    if (RTC_get_time(&time) != RTC_DRIVER_OK) {
        events |= APP_EVENT_RTC_DRIVER_GETTIME_ERROR_M;
        return;
    }

    *hours = time.hours;
    *minutes = time.minutes;
    *seconds = time.seconds;
}

void RTC_Service_GetDate(uint8_t *day, uint8_t *month, uint8_t *year, uint8_t *century) {
    RTC_Date date = {0};

    if (RTC_get_date(&date) != RTC_DRIVER_OK) {
        events |= APP_EVENT_RTC_DRIVER_GETDATE_ERROR_M;
        return;
    }

    *day = date.day;
    *month = date.month;
    *year = date.year;
    *century = date.century;
}

void RTC_Service_GetAlarmTime(uint8_t *hours, uint8_t *minutes) {
    RTC_Time time = {0};

    if (RTC_get_alarm_time(&time) != RTC_DRIVER_OK) {
        events |= APP_EVENT_RTC_DRIVER_GETALARM_ERROR_M;
        return;
    }

    *hours = time.hours;
    *minutes = time.minutes;
}

void RTC_Service_EnableAlarm(void) {
    RTC_alarm_enable();
}

void RTC_Service_DisableAlarm(void) {
    RTC_alarm_disable();
}

uint32_t RTC_Service_GetEvents(void) {
    uint32_t current_events = events;
    events = APP_EVENT_NONE_M;
    return current_events;
}

uint32_t RTC_Service_GetIRQEvents(void) {
    uint32_t events = APP_EVENT_NONE_M;
    if (RTC_get_alarm_flag()) {
        events |= APP_EVENT_RTC_IS_ALARM_M;
    }
    return events;
}