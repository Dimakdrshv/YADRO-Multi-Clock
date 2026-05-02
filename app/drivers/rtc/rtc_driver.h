#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum __rtc_status {
    RTC_DRIVER_OK = 0u,
    RTC_DRIVER_ERROR
} RTC_Status;

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t dow;
} RTC_Time;

typedef struct {
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t century;
} RTC_Date;

RTC_Status RTC_init(void);

RTC_Status RTC_set_time(const RTC_Time *time);
RTC_Status RTC_get_time(RTC_Time *time);

RTC_Status RTC_set_date(const RTC_Date *date);
RTC_Status RTC_get_date(RTC_Date *date);

RTC_Status RTC_set_alarm_time(const RTC_Time *time);
RTC_Status RTC_get_alarm_time(RTC_Time *time);
void RTC_alarm_enable(void);
void RTC_alarm_disable(void);

void RTC_IRQHandler(void);
uint32_t RTC_get_alarm_flag(void);

