#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum __clock_status {
    CLOCK_INIT_STATUS_OK = 0u,
    CLOCK_INIT_STATUS_ERROR
} Clock_Status;

typedef enum __clock_system_source {
    CLOCK_OSC32M = 0u,
    CLOCK_HSI32M = 1u,
    CLOCK_OSC32K = 2u,
    CLOCK_LSI32K = 3u
} Clock_SystemSource;

Clock_Status Clock_init(void);

void Clock_enable_PAD_Config(void);
void Clock_enable_EPIC(void);
void Clock_enable_UART0(void);
void Clock_enable_I2C1(void);
void Clock_enable_RTC(void);
void Clock_enable_TIMER32_1(void);
void Clock_enable_BUZZER(void);
void Clock_enable_DHT11(void);

uint32_t Clock_get_UART0_FREQ(void);
uint32_t Clock_get_TIMER32_1_FREQ(void);