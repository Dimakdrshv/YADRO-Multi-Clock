#pragma once

#include <stdint.h>
#include <stdbool.h>

void LCD_Service_ShowStartup(void);

void LCD_Service_ShowClock(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year);

void LCD_Service_ShowAlarm(uint8_t hours, uint8_t minutes);

void LCD_Service_ShowTemperatureHumidity(uint8_t temperature, uint8_t humidity);

void LCD_Service_DisplayOn(void);

void LCD_Service_DisplayOff(void);

void LCD_Service_Clear(void);

uint32_t LCD_Service_GetEvents(void);

