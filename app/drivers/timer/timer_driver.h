#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum __timer_status {
    TIMER_DRIVER_OK = 0,
    TIMER_DRIVER_ERROR
} TIMER_Status;

TIMER_Status TIMER_init(void);

void TIMER_IRQHandler(void);

uint32_t TIMER_get_overflow_flag(void);