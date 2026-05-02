#include "drivers/buzzer/buzzer_driver.h"
#include "platform/clock_control.h"
#include "platform/pad_control.h"
#include "platform/io_mapping.h"
#include <gpio.h>

static void BUZZER_set_direction(void) {
    BUZZER_BASE->DIRECTION_OUT |= BUZZER_PIN_MASK;
}

BUZZER_Status BUZZER_init(void) {
    Clock_enable_BUZZER();
    PAD_BUZZER_settings();
    BUZZER_set_direction();

    return BUZZER_DRIVER_OK;
}

void BUZZER_set_value(bool value) {
    BUZZER_BASE->OUTPUT = (BUZZER_BASE->OUTPUT & ~BUZZER_PIN_MASK) | (value << BUZZER_PIN); 
}