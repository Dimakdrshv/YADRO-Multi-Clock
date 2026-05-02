#pragma once

#include <stdbool.h>
#include <stdint.h>

void BUZZER_Service_SetHigh(void);

void BUZZER_Service_SetLow(void);

uint32_t BUZZER_Service_GetEvents(void);