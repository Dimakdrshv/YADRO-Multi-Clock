#pragma once

#include <stdint.h>
#include <stdbool.h>

void MTIMER_init(void);

uint64_t MTIMER_get_TIME(void);

void MTIMER_delay_us(uint64_t us);
void MTIMER_delay_ms(uint64_t ms);

