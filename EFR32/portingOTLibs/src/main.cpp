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
    // subCycleTime += 1;
    // subCycleTime = (256U == subCycleTime) ? 0 : subCycleTime;
    OTV0P2BASE::tickSubCycle();
}
}

int main(void)
{
    /* Chip errata */
    CHIP_Init();

    if(OTV0P2BASE::setupEmulated2sSubCycle()) { DEBUG_BREAK; }

    setupGPIO();
    auto ledState = false;
    /* Infinite loop */
    while (1) {
        if(255U == OTV0P2BASE::getSubCycleTime()) {
          ledState = !ledState;
        //   setLED<gpioPortF, 4>(ledState);
          setLED<gpioPortF, 5>(!ledState);
        }
    }
}
