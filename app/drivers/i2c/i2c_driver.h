#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum __i2c_status {
    I2C_DRIVER_OK = 0,
    I2C_DRIVER_ERROR
} I2C_Status;

I2C_Status I2C_init(void);

I2C_Status I2C_write(uint8_t byte);