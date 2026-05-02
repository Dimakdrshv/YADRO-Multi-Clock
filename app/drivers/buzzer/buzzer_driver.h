#pragma once

#include <stdbool.h>

typedef enum __buzzer_status {
    BUZZER_DRIVER_OK = 0,
    BUZZER_DRIVER_ERROR
} BUZZER_Status;

BUZZER_Status BUZZER_init(void);

void BUZZER_set_value(bool value);
