#include "buzzer_service.h"
#include "apps/event/app_events.h"
#include "drivers/buzzer/buzzer_driver.h"

static uint32_t events = APP_EVENT_NONE_M;

void BUZZER_Service_SetHigh(void) {
    BUZZER_set_value(true);
}

void BUZZER_Service_SetLow(void) {
    BUZZER_set_value(false);
}

uint32_t BUZZER_Service_GetEvents(void) {
    uint32_t current_events = events;
    events = APP_EVENT_NONE_M;
    return current_events;
}