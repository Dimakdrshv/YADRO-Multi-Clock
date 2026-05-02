#pragma once

#include <stdint.h>

void RTC_Service_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void RTC_Service_SetDate(uint8_t day, uint8_t month, uint8_t year, uint8_t century);
void RTC_Service_SetAlarmTime(uint8_t hours, uint8_t minutes);

void RTC_Service_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);
void RTC_Service_GetDate(uint8_t *day, uint8_t *month, uint8_t *year, uint8_t *century);
void RTC_Service_GetAlarmTime(uint8_t *hours, uint8_t *minutes);

void RTC_Service_EnableAlarm(void);
void RTC_Service_DisableAlarm(void);

uint32_t RTC_Service_GetEvents(void);
uint32_t RTC_Service_GetIRQEvents(void);