#===================================================================================================
# MIK32 Shared library
#===================================================================================================

add_library(mik32_shared
    mik32v2-shared/runtime/crt0.S
)

target_include_directories(mik32_shared
    PUBLIC
        mik32v2-shared/periphery
        mik32v2-shared/include
        mik32v2-shared/libs
)

#===================================================================================================
# MIK32 HAL library
#===================================================================================================

add_library(mik32_hal
    mik32-hal/core/Source/mik32_hal_scr1_timer.c
    mik32-hal/peripherals/Source/mik32_hal_adc.c
    mik32-hal/peripherals/Source/mik32_hal_crc32.c
    mik32-hal/peripherals/Source/mik32_hal_crypto.c
    mik32-hal/peripherals/Source/mik32_hal_dac.c
    mik32-hal/peripherals/Source/mik32_hal_dma.c
    mik32-hal/peripherals/Source/mik32_hal_eeprom.c
    mik32-hal/peripherals/Source/mik32_hal_gpio.c
    mik32-hal/peripherals/Source/mik32_hal_i2c.c
    mik32-hal/peripherals/Source/mik32_hal_irq.c
    mik32-hal/peripherals/Source/mik32_hal_otp.c
    mik32-hal/peripherals/Source/mik32_hal_pcc.c
    mik32-hal/peripherals/Source/mik32_hal_rtc.c
    mik32-hal/peripherals/Source/mik32_hal_spi.c
    mik32-hal/peripherals/Source/mik32_hal_spifi.c
    mik32-hal/peripherals/Source/mik32_hal_timer16.c
    mik32-hal/peripherals/Source/mik32_hal_timer32.c
    mik32-hal/peripherals/Source/mik32_hal_tsens.c
    mik32-hal/peripherals/Source/mik32_hal_usart.c
    mik32-hal/peripherals/Source/mik32_hal_wdt.c
    mik32-hal/peripherals/Source/mik32_hal.c
    mik32-hal/utilities/Source/mik32_hal_spifi_w25.c
    mik32-hal/utilities/Source/mik32_hal_ssd1306.c
)

target_include_directories(mik32_hal
    PUBLIC
        mik32-hal/peripherals/Include

    PRIVATE
        mik32-hal/core/Include
        mik32-hal/utilities/Include
)

target_link_libraries(mik32_hal
    PUBLIC
        mik32_shared
)
