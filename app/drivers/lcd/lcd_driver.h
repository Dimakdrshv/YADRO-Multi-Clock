#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum __lcd_status {
    LCD_DRIVER_OK = 0,
    LCD_DRIVER_ERROR
} LCD_Status;

LCD_Status LCD_init(void);

LCD_Status LCD_clear(void);
LCD_Status LCD_home(void);

LCD_Status LCD_set_cursor(uint8_t row, uint8_t col);

LCD_Status LCD_write_char(char c);
LCD_Status LCD_write_string(const char *str);

LCD_Status LCD_display_on(void);
LCD_Status LCD_display_off(void);
