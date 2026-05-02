#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum __dht11_status {
    DHT11_DRIVER_OK = 0u,
    DHT11_DRIVER_ERROR
} DHT11_Status;

DHT11_Status DHT11_init(void);

DHT11_Status DHT11_read(uint32_t* data);