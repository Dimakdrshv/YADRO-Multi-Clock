#include "timer_driver.h"
#include <timer32.h>
#include "platform/clock_control.h"
#include "platform/io_mapping.h"
#include "platform/epic_irq.h"

#define TIMER_PRESCALE       (32000u)
#define TIMER_INTERRUPT_FREQ (1u)
#define TIMER_FREQ           (Clock_get_TIMER32_1_FREQ())
#define TIMER_MAX_VALUE      (TIMER_FREQ / TIMER_INTERRUPT_FREQ / TIMER_PRESCALE)

static uint32_t timer_overflow_pending = 0u;

static void TIMER_enable(void) {
    TIMER32_1_BASE->ENABLE = TIMER32_ENABLE_TIM_EN_M;
}

static void TIMER_disable(void) {
    TIMER32_1_BASE->ENABLE &= ~TIMER32_ENABLE_TIM_EN_M;
}

static void TIMER_set_top(void) {
    TIMER32_1_BASE->TOP = TIMER_MAX_VALUE - 1u;
}

static void TIMER_set_prescale(void) {
    TIMER32_1_BASE->PRESCALER = TIMER_PRESCALE - 1u;
}

static void TIMER_set_control(void) {
    TIMER32_1_BASE->CONTROL = TIMER32_CONTROL_CLOCK_PRESCALER_M | TIMER32_CONTROL_MODE_UP_M;
}

static void TIMER_overflow_interrupt_enable(void) {
    TIMER32_1_BASE->INT_CLEAR = TIMER32_INT_OVERFLOW_M;;
    TIMER32_1_BASE->INT_MASK = TIMER32_INT_OVERFLOW_M;
}

TIMER_Status TIMER_init(void) {
    Clock_enable_TIMER32_1();

    TIMER_disable();
    TIMER_set_top();
    TIMER_set_prescale();
    TIMER_set_control();
    TIMER_overflow_interrupt_enable();
    TIMER_enable();

    EPIC_IRQ_enable(TIMER32_1_IRQ_INDEX);

    return TIMER_DRIVER_OK;
}

void TIMER_IRQHandler(void) {
    timer_overflow_pending = 1u;
    TIMER32_1_BASE->INT_CLEAR = TIMER32_INT_OVERFLOW_M;
}

uint32_t TIMER_get_overflow_flag(void) {
    if (timer_overflow_pending == 0u) {
        return 0u;
    }

    timer_overflow_pending = 0u;
    return 1u;
}