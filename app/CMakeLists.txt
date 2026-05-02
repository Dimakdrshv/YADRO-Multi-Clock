set(APP_NAME ${CMAKE_PROJECT_NAME})

set(APP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(APP_SOURCES
    ${APP_ROOT}/main.c

    ${APP_ROOT}/apps/controller/app_controller.c
    ${APP_ROOT}/apps/init/app_init.c

    ${APP_ROOT}/services/buzzer/buzzer_service.c
    ${APP_ROOT}/services/command/command_service.c
    ${APP_ROOT}/services/dht11/dht11_service.c
    ${APP_ROOT}/services/lcd/lcd_service.c
    ${APP_ROOT}/services/rtc/rtc_service.c
    ${APP_ROOT}/services/timer/timer_service.c

    ${APP_ROOT}/drivers/buzzer/buzzer_driver.c
    ${APP_ROOT}/drivers/dht11/dht11_driver.c
    ${APP_ROOT}/drivers/i2c/i2c_driver.c
    ${APP_ROOT}/drivers/lcd/lcd_driver.c
    ${APP_ROOT}/drivers/rtc/rtc_driver.c
    ${APP_ROOT}/drivers/timer/timer_driver.c
    ${APP_ROOT}/drivers/uart/uart_driver.c

    ${APP_ROOT}/platform/clock_control.c
    ${APP_ROOT}/platform/epic_irq.c
    ${APP_ROOT}/platform/irq_dispatcher.c
    ${APP_ROOT}/platform/mtimer.c
    ${APP_ROOT}/platform/pad_control.c
    ${APP_ROOT}/platform/system.c
)

add_executable(${APP_NAME}
    ${APP_SOURCES}
)

target_include_directories(${APP_NAME}
    PRIVATE
        ${APP_ROOT}
        ${APP_ROOT}/apps
        ${APP_ROOT}/services
        ${APP_ROOT}/drivers
        ${APP_ROOT}/platform
)

target_link_libraries(${APP_NAME}
    PRIVATE
        mik32_hal
)

target_link_directories(${APP_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/hardware/mik32v2-shared/ldscripts
)

add_custom_command(TARGET ${APP_NAME}
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${TOOLCHAIN_PREFIX}-objcopy ARGS -O binary ${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX_C} ${APP_NAME}.bin
    COMMAND ${TOOLCHAIN_PREFIX}-objcopy ARGS -O ihex ${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX_C} ${APP_NAME}.hex
)
