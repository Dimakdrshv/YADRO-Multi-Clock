#include "platform/system.h"
#include "platform/irq_dispatcher.h"
#include "apps/init/app_init.h"
#include "apps/controller/app_controller.h"


int main (void) {
    System_init(); 
    App_Init();
    
    while (1)
    {
        App_Controller_Process();
    }
}

void trap_handler(void) {
    IRQ_Dispatch();
}