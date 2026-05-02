#include "timer_service.h"
#include "drivers/timer/timer_driver.h"
#include "apps/event/app_events.h"

uint32_t TIMER_Service_GetIRQEvents(void) {
    uint32_t events = APP_EVENT_NONE_M;
    if (TIMER_get_overflow_flag()) {
        events |= APP_EVENT_TIMER32_1_IS_OVERFLOW_M;
    }

    return events;
}