#include "i2c_driver.h"
#include <i2c.h>
#include "platform/io_mapping.h"
#include "platform/pad_control.h"
#include "platform/clock_control.h"

#define LCD_ADDR            (0x27u << 1u)

#define I2C_TIMING_SCLL     (0x27u)
#define I2C_TIMING_SCLH     (0x27u)
#define I2C_TIMING_SDADEL   (0x2u)
#define I2C_TIMING_SCLDEL   (0x9u)
#define I2C_TIMING_PRESC    (0x3u)

#define I2C_TRANSMIT_TIMEOUT (100000u) 

static void I2C_disable(void) {
    I2C_1_BASE->CR1 &= ~I2C_CR1_PE_M;

    // wait for disable and reset (datasheet)
    uint8_t delay = 3u;
    while (delay--) {
        __asm__ volatile ("nop");
    }
}

static void I2C_clear_control_registers(void) {
    I2C_1_BASE->CR1 = 0u;
    I2C_1_BASE->CR2 = 0u;
}

static void I2C_set_timing(void) {
    I2C_1_BASE->TIMINGR = I2C_TIMINGR_SCLH(I2C_TIMING_SCLH) 
                        | I2C_TIMINGR_SCLL(I2C_TIMING_SCLL) 
                        | I2C_TIMINGR_SDADEL(I2C_TIMING_SDADEL) 
                        | I2C_TIMINGR_SCLDEL(I2C_TIMING_SCLDEL)
                        | I2C_TIMINGR_PRESC(I2C_TIMING_PRESC);
}

static void I2C_enable(void) {
    I2C_1_BASE->CR1 |= I2C_CR1_PE_M;
}

I2C_Status I2C_init(void) {

    Clock_enable_I2C1();
    PAD_I2C1_settings();

    I2C_disable();
    I2C_clear_control_registers();
    I2C_set_timing();
    I2C_enable();

    return I2C_DRIVER_OK;

}

static bool I2C_wait_not_BUSY(uint32_t timeout) {
    while (timeout) {
        if (!(I2C_1_BASE->ISR & I2C_ISR_BUSY_M)) {
            return true;
        }
        --timeout;
    }
    return false;
}

static bool I2C_wait_STOPF(uint32_t timeout) {
    while (timeout) {
        if (I2C_1_BASE->ISR & I2C_ISR_STOPF_M) {
            I2C_1_BASE->ICR = I2C_ICR_STOPCF_M;
            return true;
        }
        --timeout;
    }
    return false;
}

static bool I2C_wait_TXIS_or_NACK(uint32_t timeout) {
    while (timeout) {
        if (I2C_1_BASE->ISR & I2C_ISR_TXIS_M) {
            return true;
        }
        if (I2C_1_BASE->ISR & I2C_ISR_NACKF_M) {
            I2C_1_BASE->ICR = I2C_ICR_NACKCF_M;
            I2C_wait_STOPF(I2C_TRANSMIT_TIMEOUT); // 100% after nack 80 page prelast sentence 
            return false;
        }
        --timeout;
    }
    return false;
}

static void I2C_load_config(void) {
    I2C_1_BASE->CR2 = 0u;
    I2C_1_BASE->CR2 |= I2C_CR2_SADD(LCD_ADDR);
    I2C_1_BASE->CR2 |= I2C_CR2_NBYTES(1u);
    I2C_1_BASE->CR2 |= I2C_CR2_AUTOEND_M;
    I2C_1_BASE->CR2 &= ~I2C_CR2_RD_WRN_M;
    I2C_1_BASE->CR2 |= I2C_CR2_START_M;
}

static void I2C_recover(void) {
    I2C_disable();
    I2C_clear_control_registers();
    I2C_set_timing();
    I2C_enable();
}

I2C_Status I2C_write(uint8_t byte) {
    if (!I2C_wait_not_BUSY(I2C_TRANSMIT_TIMEOUT)) {
        I2C_recover();
        return I2C_DRIVER_ERROR;
    }

    I2C_load_config();

    if (!I2C_wait_TXIS_or_NACK(I2C_TRANSMIT_TIMEOUT)) {
        I2C_recover();
        return I2C_DRIVER_ERROR;
    }

    I2C_1_BASE->TXDR = byte;

    if (!I2C_wait_STOPF(I2C_TRANSMIT_TIMEOUT)) {
        I2C_recover();
        return I2C_DRIVER_ERROR;
    }

    return I2C_DRIVER_OK;
}