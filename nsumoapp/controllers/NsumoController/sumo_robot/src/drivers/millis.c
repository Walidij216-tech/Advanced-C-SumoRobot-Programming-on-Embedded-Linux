#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/drivers/millis.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/defines.h"
#include "/home/walid/Desktop/msp430-gcc/lib/gcc/msp430-elf/9.3.1/plugin/include/config/msp430/msp430.h"

static volatile uint32_t watchdog_interrupt_cnt = 0;

INTERRUPT_FUNCTION(WDT_VECTOR) isr_watchdog(void)
{
    watchdog_interrupt_cnt++;
}

uint32_t millis()
{
    // Disable interrupts while retrieving the counter
    IE1 &= ~WDTIE;
    // Divide by two because the watchdog timer triggers every 0.5 ms
    const uint32_t ms = watchdog_interrupt_cnt / 2;
    IE1 |= WDTIE;
    return ms;
}
