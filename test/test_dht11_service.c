#include "unity.h"

#include "dht11_service.h"
#include "apps/event/app_events.h"

#include "mock_dht11_driver.h"

void setUp(void)
{

}

void tearDown(void) 
{

}

void test_DHT11_normal_parse_temperature_and_humidity(void)
{
    uint8_t temperature;
    uint8_t humidity;

    uint32_t data = (55u << 24u) | (23u << 8u);

    DHT11_read_ExpectAndReturn(NULL, DHT11_DRIVER_OK);
    DHT11_read_IgnoreArg_data();
    DHT11_read_ReturnThruPtr_data(&data);

    DHT11_Service_GetTemperatureHumidity(&temperature, &humidity);

    TEST_ASSERT_EQUAL_UINT8(23, temperature);
    TEST_ASSERT_EQUAL_UINT8(55, humidity);

    TEST_ASSERT_BITS
    (
        APP_EVENT_NONE_M,
        APP_EVENT_NONE_M,
        DHT11_Service_GetEvents()
    );
}

void test_DHT11_INVALID_ARGUMENTS_event(void)
{
    uint8_t temperature;

    DHT11_Service_GetTemperatureHumidity(&temperature, NULL);

    TEST_ASSERT_BITS
    (
        APP_EVENT_DHT11_DRIVER_INVALIDARG_ERROR_M,
        APP_EVENT_DHT11_DRIVER_INVALIDARG_ERROR_M,
        DHT11_Service_GetEvents()
    );
}

void test_DHT11_READ_ERROR_event(void)
{
    uint8_t temperature;
    uint8_t humidity;

    uint32_t data = (55u << 24u) | (23u << 8u);

    DHT11_read_ExpectAndReturn(NULL, DHT11_DRIVER_ERROR);
    DHT11_read_IgnoreArg_data();
    DHT11_read_ReturnThruPtr_data(&data);

    DHT11_Service_GetTemperatureHumidity(&temperature, &humidity);

    TEST_ASSERT_BITS
    (
        APP_EVENT_DHT11_DRIVER_READ_ERROR_M,
        APP_EVENT_DHT11_DRIVER_READ_ERROR_M,
        DHT11_Service_GetEvents()
    );
}