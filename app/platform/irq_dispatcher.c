// Dispatches interrupts to the correct driver handler.
// Checks pending IRQs and calls the corresponding interrupt handler.

#include "irq_dispatcher.h"
#include "epic_irq.h"
#include "io_mapping.h"
#include "drivers/uart/uart_driver.h"
#include "drivers/timer/timer_driver.h"
#include "drivers/rtc/rtc_driver.h"

void IRQ_Dispatch(void) {
    if (EPIC_IRQ_IsPending(UART_0_IRQ_INDEX)) {
        UART_IRQHandler();
        EPIC_IRQ_clear(UART_0_IRQ_INDEX); // byte receive
    }
    if (EPIC_IRQ_IsPending(RTC_IRQ_INDEX)) {
        RTC_IRQHandler();
        EPIC_IRQ_clear(RTC_IRQ_INDEX); // alarm 
    }
    if (EPIC_IRQ_IsPending(TIMER32_1_IRQ_INDEX)) {
        TIMER_IRQHandler();
        EPIC_IRQ_clear(TIMER32_1_IRQ_INDEX); // 1 sec interrupt
    }
}
