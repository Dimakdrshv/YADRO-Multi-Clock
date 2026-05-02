// Initializes basic system hardware.
// Enables system clocks, PAD subsystem, and interrupt controller.

#include "system.h"
#include "clock_control.h"
#include "pad_control.h"
#include "epic_irq.h"
#include "mtimer.h"

void System_init(void) {
    Clock_init();
    MTIMER_init();
    PAD_init();
    EPIC_IRQ_init();
}