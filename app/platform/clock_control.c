// Controls system and peripheral clocks.
// Provides functions to enable clocks and get current clock frequencies.

#include "clock_control.h"

#include <power_manager.h>
#include "io_mapping.h"
#include <wakeup.h>

#define CLOCK_FREQ_OSC32M     (32000000u)
#define CLOCK_FREQ_HSI32M     (32000000u)
#define CLOCK_FREQ_OSC32K     (32768u)
#define CLOCK_FREQ_LSI32K     (32768u)

#define CLOCK_STARTUP_TIMEOUT (1000000u)

static void Clock_enable_OSC32M(void) {
    WU->CLOCKS_SYS &= ~WU_CLOCKS_SYS_OSC32M_EN_M;
}

static void Clock_enable_OSC32K(void) {
    WU->CLOCKS_BU &= ~WU_CLOCKS_BU_OSC32K_EN_M;
}

static bool Clock_wait_OSC32M_ready(uint32_t timeout) {
    while (timeout) {
        if (PM->FREQ_STATUS & PM_FREQ_MASK_OSC32M_M) {
            return true;
        }
        --timeout;
    }
    return false;
}

static bool Clock_wait_OSC32K_ready(uint32_t timeout) {
    while (timeout) {
        if (PM->FREQ_STATUS & PM_FREQ_MASK_OSC32K_M) {
            return true;
        }
        --timeout;
    }
    return false;
}

static void Clock_AHB_set(Clock_SystemSource source) {
    PM->AHB_CLK_MUX = source;
}

static void DIV_set(uint32_t div_ahb, uint32_t div_apb_m, uint32_t div_apb_p) {
    PM->DIV_AHB = div_ahb;
    PM->DIV_APB_M = div_apb_m;
    PM->DIV_APB_P = div_apb_p;
}

static void Clock_set_RTC_clock(void) {
    WU->CLOCKS_BU = (WU->CLOCKS_BU & ~WU_CLOCKS_BU_RTC_CLK_MUX_M) | WU_CLOCKS_BU_RTC_CLK_MUX_OSC32K_M;
}

Clock_Status Clock_init(void) {
    Clock_enable_OSC32M();
    Clock_enable_OSC32K();

    if (!Clock_wait_OSC32M_ready(CLOCK_STARTUP_TIMEOUT)) {
        return CLOCK_INIT_STATUS_ERROR;
    }
    if (!Clock_wait_OSC32K_ready(CLOCK_STARTUP_TIMEOUT)) {
        return CLOCK_INIT_STATUS_ERROR;
    }

    Clock_AHB_set(CLOCK_OSC32M);
    DIV_set(0u, 0u, 0u);

    Clock_set_RTC_clock();

    return CLOCK_INIT_STATUS_OK;
}

void Clock_enable_PAD_Config(void) {
    PM->CLK_APB_M_SET = PM_CLOCK_APB_M_PAD_CONFIG_M;
}

void Clock_enable_EPIC(void) {
    PM->CLK_APB_M_SET = PM_CLOCK_APB_M_EPIC_M;
}

void Clock_enable_UART0(void) {
    PM->CLK_APB_P_SET = UART_0_CLOCK_MASK;
}

void Clock_enable_I2C1(void) {
    PM->CLK_APB_P_SET = I2C_1_CLOCK_MASK;
}

void Clock_enable_RTC(void) {
    PM->CLK_APB_M_SET = RTC_CLOCK_MASK;
}

void Clock_enable_TIMER32_1(void) {
    PM->CLK_APB_P_SET = TIMER32_1_CLOCK_MASK;
}

void Clock_enable_BUZZER(void) {
    PM->CLK_APB_P_SET = BUZZER_CLOCK_MASK;
}

void Clock_enable_DHT11(void) {
    PM->CLK_APB_P_SET = DHT11_CLOCK_MASK;
}

static uint32_t Clock_get_SYS_FREQ(Clock_SystemSource source) {
    switch (source) {
        case CLOCK_OSC32M: return CLOCK_FREQ_OSC32M;
        case CLOCK_HSI32M: return CLOCK_FREQ_HSI32M;
        case CLOCK_OSC32K: return CLOCK_FREQ_OSC32K;
        case CLOCK_LSI32K: return CLOCK_FREQ_LSI32K;
        default: return 0u;
    }
}

static uint32_t Clock_get_AHB_FREQ(void) {
    return Clock_get_SYS_FREQ((Clock_SystemSource)(PM->AHB_CLK_MUX & 0x3u)) / (PM->DIV_AHB + 1u);
}

static uint32_t Clock_get_APB_P_FREQ(void) {
    return Clock_get_AHB_FREQ() / (PM->DIV_APB_P + 1u);
}

uint32_t Clock_get_UART0_FREQ(void) {
    return Clock_get_APB_P_FREQ();
}

uint32_t Clock_get_TIMER32_1_FREQ(void) {
    return Clock_get_APB_P_FREQ();
}