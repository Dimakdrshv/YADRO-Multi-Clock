// https://github.com/syntacore/scr1/blob/master/docs/scr1_eas.pdf

#include "mtimer.h"
#include "io_mapping.h"
#include <scr1_timer.h>

#define TIMER_DIV_DEFAULT (0u)

static void MTIMER_enable(void) {
    MTIMER_BASE->TIMER_CTRL |= SCR1_TIMER_CTRL_ENABLE_M;
}

static void MTIMER_load_config(void) {
    MTIMER_BASE->TIMER_CTRL = SCR1_TIMER_CTRL_CLKSRC_INTERNAL_M;
    MTIMER_BASE->TIMER_DIV = TIMER_DIV_DEFAULT; 
}

void MTIMER_init(void) {
    MTIMER_load_config();
    MTIMER_enable();
}

uint64_t MTIMER_get_TIME(void) {
    uint32_t lo;
    uint32_t hi1;
    uint32_t hi2;
    
    do {
        hi1 = MTIMER_BASE->MTIMEH;
        lo = MTIMER_BASE->MTIME;
        hi2 = MTIMER_BASE->MTIMEH;
    } while (hi1 != hi2);

    return ((uint64_t)hi1 << 32u) | lo; 
}

#define MTIMER_TICKS_PER_US 32u

void MTIMER_delay_us(uint64_t us) {
    uint64_t end = MTIMER_get_TIME() + us * MTIMER_TICKS_PER_US;

    while (MTIMER_get_TIME() < end)
        ;
}

void MTIMER_delay_ms(uint64_t ms) {
    while (ms--) {
        MTIMER_delay_us(1000u);
    }
}