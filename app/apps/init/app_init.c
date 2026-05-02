#include "drivers/uart/uart_driver.h"
#include "drivers/i2c/i2c_driver.h"
#include "drivers/lcd/lcd_driver.h"
#include "drivers/rtc/rtc_driver.h"
#include "drivers/timer/timer_driver.h"
#include "drivers/buzzer/buzzer_driver.h"
#include "drivers/dht11/dht11_driver.h"

void App_Init(void) {
    if (UART_init() != UART_DRIVER_OK) {
        while(1);
    }
    if (RTC_init() != RTC_DRIVER_OK) {
        while(1);
    }
    if (TIMER_init() != TIMER_DRIVER_OK) {
        while(1);
    }
    if (I2C_init() != I2C_DRIVER_OK) {
        while(1);
    }
    if (LCD_init() != LCD_DRIVER_OK) {
        while(1);
    }
    if (BUZZER_init() != BUZZER_DRIVER_OK) {
        while(1);
    }
    if (DHT11_init() != DHT11_DRIVER_OK) {
        while(1);
    }
}