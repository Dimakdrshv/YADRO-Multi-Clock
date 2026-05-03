// Initializes basic system hardware.
// Enables system clocks, PAD subsystem, and interrupt controller.

#include "system.h"
#include "clock_control.h"
#include "pad_control.h"
#include "epic_irq.h"
#include "mtimer.h"

void System_init(void) {
    if (Clock_init() != CLOCK_INIT_STATUS_OK) {
        while(1);
    }
    MTIMER_init();
    PAD_init();
    EPIC_IRQ_init();
}