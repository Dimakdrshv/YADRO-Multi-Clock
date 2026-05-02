#pragma once

#include <stdint.h>
#include <stdbool.h>

void DHT11_Service_GetTemperatureHumidity(uint8_t* temperature, uint8_t* humidity);

uint32_t DHT11_Service_GetEvents(void);