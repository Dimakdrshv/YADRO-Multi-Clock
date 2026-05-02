// <*!> interrupts - EPIC IRQ handling
#pragma once

#include <stdbool.h>
#include <stdint.h>

void EPIC_IRQ_init(void);

void EPIC_IRQ_enable(uint32_t irq_index);

void EPIC_IRQ_disable(uint32_t irq_index);

void EPIC_IRQ_clear(uint32_t irq_index);

uint32_t EPIC_IRQ_GetRawStatus(void);

bool EPIC_IRQ_IsPending(uint32_t irq_index);
