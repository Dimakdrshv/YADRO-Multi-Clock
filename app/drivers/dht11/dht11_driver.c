#include "dht11_driver.h"
#include "platform/clock_control.h"
#include "platform/pad_control.h"
#include "platform/io_mapping.h"
#include "platform/mtimer.h"
#include <gpio.h>

#define DHT11_WAIT_TIMEOUT_TICKS (50000u)

#define DHT11_HIGH_PIN_LEVEL  (true)
#define DHT11_LOW_PIN_LEVEL   (false)

#define DHT11_DATA_BYTES      (5u)
#define DHT11_PARITY_BYTE     (4u)
#define DHT11_BITS_IN_BYTE    (8u)

#define DHT11_POWER_STABILIZE_DELAY_MS    (1000u)
#define DHT11_START_SIGNAL_DELAY_MS       (20u)
#define DHT11_START_RELEASE_DELAY_US      (40u)
#define DHT11_RESPONSE_LOW_DELAY_US       (80u)
#define DHT11_RESPONSE_HIGH_DELAY_US      (80u)
#define DHT11_BIT_SAMPLE_DELAY_US         (30u)

static void DHT11_direction_input(void) {
    DHT11_BASE->DIRECTION_IN = DHT11_PIN_MASK;
} 

static void DHT11_direction_output(void) {
    DHT11_BASE->DIRECTION_OUT = DHT11_PIN_MASK;
}

DHT11_Status DHT11_init(void) {
    Clock_enable_DHT11();
    PAD_DHT11_settings();

    DHT11_direction_input();
    MTIMER_delay_ms(DHT11_POWER_STABILIZE_DELAY_MS);

    return DHT11_DRIVER_OK;
}

static void DHT11_pin_clear(void) {
    DHT11_BASE->CLEAR = DHT11_PIN_MASK;
}

static void DHT11_pin_set(void) {
    DHT11_BASE->SET = DHT11_PIN_MASK;
}

static bool DHT11_pin_read(void) {
    return DHT11_BASE->STATE & DHT11_PIN_MASK;
}

static DHT11_Status DHT11_wait_pin_level(bool level) {
    uint64_t start_timing = MTIMER_get_TIME();

    while (DHT11_pin_read() != level) {
        if (MTIMER_get_TIME() - start_timing > DHT11_WAIT_TIMEOUT_TICKS) {
            return DHT11_DRIVER_ERROR;
        }
    }
    return DHT11_DRIVER_OK;
}

static void DHT11_send_start_signal(void) {
    DHT11_direction_output();
    DHT11_pin_clear();
    MTIMER_delay_ms(DHT11_START_SIGNAL_DELAY_MS);
    DHT11_pin_set();
    MTIMER_delay_us(DHT11_START_RELEASE_DELAY_US);
    DHT11_direction_input();
}

static DHT11_Status DHT11_read_byte(uint8_t *byte) {
    if (byte == 0) {
        return DHT11_DRIVER_ERROR;
    }

    *byte = 0u;

    for (uint8_t i = 0u; i < DHT11_BITS_IN_BYTE; i++) {
        if (DHT11_wait_pin_level(DHT11_HIGH_PIN_LEVEL) != DHT11_DRIVER_OK) {
            return DHT11_DRIVER_ERROR;
        }
        MTIMER_delay_us(DHT11_BIT_SAMPLE_DELAY_US);

        if (DHT11_pin_read()) {
            *byte |= (uint8_t)(1u << (DHT11_BITS_IN_BYTE - 1u - i));
        }

        if (DHT11_wait_pin_level(DHT11_LOW_PIN_LEVEL) != DHT11_DRIVER_OK) {
            return DHT11_DRIVER_ERROR;
        }
    }
    return DHT11_DRIVER_OK;
}

static DHT11_Status DHT11_read_data(uint8_t data[DHT11_DATA_BYTES]) {
    if (data == 0) {
        return DHT11_DRIVER_ERROR;
    }

    DHT11_send_start_signal();

    if (DHT11_wait_pin_level(DHT11_LOW_PIN_LEVEL) != DHT11_DRIVER_OK) {
        return DHT11_DRIVER_ERROR;
    }
    MTIMER_delay_us(DHT11_RESPONSE_LOW_DELAY_US);

    if (!DHT11_pin_read()) {
        return DHT11_DRIVER_ERROR;
    }
    MTIMER_delay_us(DHT11_RESPONSE_HIGH_DELAY_US);

    for (uint8_t i = 0u; i < DHT11_DATA_BYTES; i++) {
        if (DHT11_read_byte(&data[i]) != DHT11_DRIVER_OK) {
            return DHT11_DRIVER_ERROR;
        }
    }

    if (data[DHT11_PARITY_BYTE] != (uint8_t)(data[0] + data[1] + data[2] + data[3])) {
        return DHT11_DRIVER_ERROR;
    }
    return DHT11_DRIVER_OK;
}

DHT11_Status DHT11_read(uint32_t* data) {
    if (data == 0) {
        return DHT11_DRIVER_ERROR;
    }

    uint8_t info[DHT11_DATA_BYTES];
    if (DHT11_read_data(info) != DHT11_DRIVER_OK) {
        return DHT11_DRIVER_ERROR;
    }

    *data = 0u;
    for (uint8_t i = 0u; i < DHT11_DATA_BYTES - 1u; i++) {
        *data = (*data << 8u) | info[i];
    }
    return DHT11_DRIVER_OK;
}