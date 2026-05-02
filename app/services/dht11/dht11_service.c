#include "dht11_service.h"
#include "apps/event/app_events.h"
#include "drivers/dht11/dht11_driver.h"

static uint32_t events = APP_EVENT_NONE_M;

void DHT11_Service_GetTemperatureHumidity(uint8_t* temperature, uint8_t* humidity) {
    if (temperature == 0 || humidity == 0) {
        events |= APP_EVENT_DHT11_DRIVER_INVALIDARG_ERROR_M;
        return;
    }
    uint32_t data = 0u;
    if (DHT11_read(&data) != DHT11_DRIVER_OK) {
        events |= APP_EVENT_DHT11_DRIVER_READ_ERROR_M;
        return;
    }
    *humidity = (uint8_t)((data >> 24u) & 0xFFu);
    *temperature = (uint8_t)((data >> 8u) & 0xFFu);
}

uint32_t DHT11_Service_GetEvents(void) {
    uint32_t current_events = events;
    events = APP_EVENT_NONE_M;
    return current_events;
}