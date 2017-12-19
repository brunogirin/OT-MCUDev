extern "C" {
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"

#include "hal-config.h"
}

#include "util.h"
#include "OTV0P2BASE_Util.h"
#include "OTV0P2BASE_Sleep.h"

#define DEBUG_BREAK __asm__("BKPT #0");

extern "C" {
// SysTick emulates subcycleTime
// auto subCycleTime = 0U;
void SysTick_Handler(void) {
    OTV0P2BASE::tickSubCycle();
}
}

int main(void)
{
    // constexpr auto F_CPU = 38000000U;
    /* Chip errata */
    CHIP_Init();

    if(OTV0P2BASE::setupEmulated2sSubCycle()) { DEBUG_BREAK; }
    
    // LED setup
    setupGPIO();
    auto ledFlashed = false;
    auto ledState = true;

    /* Infinite loop */
    while (1) {
        // Poll subCycleTime and delay 1 ms
        const auto subCycleTime = OTV0P2BASE::getSubCycleTime();
        if (0U == subCycleTime) { ledFlashed = false; }
        else if((255U == subCycleTime) && (!ledFlashed)) {
            ledFlashed = true;
            ledState = !ledState;
            setLED<gpioPortF, 4>(ledState);
            setLED<gpioPortF, 5>(!ledState);
        }
    }
}
