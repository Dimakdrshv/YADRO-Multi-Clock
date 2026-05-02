// <*!> interrupts .c file
// Controls the EPIC interrupt controller.

#include "epic_irq.h"

#include <epic.h>
#include <csr.h>
#include <scr1_csr_encoding.h>

#include "platform/io_mapping.h"
#include <power_manager.h>

#include "platform/clock_control.h"

void EPIC_IRQ_init(void) {
    Clock_enable_EPIC();
    set_csr(mie, MIE_MEIE);
    set_csr(mstatus, MSTATUS_MIE);
}

void EPIC_IRQ_enable(uint32_t irq_index) {
    EPIC->MASK_LEVEL_SET = (1u << irq_index);
}

void EPIC_IRQ_disable(uint32_t irq_index) {
    EPIC->MASK_LEVEL_CLEAR = (1u << irq_index);
}

void EPIC_IRQ_clear(uint32_t irq_index) {
    EPIC->CLEAR = (1u << irq_index);
}

uint32_t EPIC_IRQ_GetRawStatus(void) {
    return EPIC->RAW_STATUS;
}

bool EPIC_IRQ_IsPending(uint32_t irq_index) {
    return (EPIC_IRQ_GetRawStatus() & (1u << irq_index)) != 0u;
}