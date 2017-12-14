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
    constexpr auto F_CPU = 38000000U;
    /* Chip errata */
    CHIP_Init();

    if(OTV0P2BASE::setupEmulated2sSubCycle()) { DEBUG_BREAK; }
    
    setupGPIO();
    auto led0State = true;
    auto led1State = true;
    /* Infinite loop */
    while (1) {
        // Flip LED every time we loop. Should take ~2 s
        led0State = !led0State;
        setLED<gpioPortF, 4>(led0State);
        for(auto i = 0; i < 2000; ++i) {
            // Poll subCycleTime and delay 1 ms
            if(255U == OTV0P2BASE::getSubCycleTime()) {
                led1State = !led1State;
                setLED<gpioPortF, 5>(!led1State);
                while(255U == OTV0P2BASE::getSubCycleTime()) {}
            }
            OTV0P2BASE::delay_ms(1);
        }
    }
}
