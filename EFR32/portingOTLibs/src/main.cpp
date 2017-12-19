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

// Default speed is 38 MHz so div2 must be enabled.
constexpr auto F_CPU = 19000000U;
OTPORT::Serial serial;

extern "C" {
// SysTick emulates subcycleTime
// auto subCycleTime = 0U;
void SysTick_Handler(void) {
    OTV0P2BASE::tickSubCycle();
}
}

int main(void)
{
    /* Chip errata */
    CHIP_Init();

    if(OTV0P2BASE::setupEmulated2sSubCycle()) { DEBUG_BREAK; }

    // Setup clocks
    OTPORT::setupClocks();
    
    // LED setup
    OTPORT::setupGPIO();
    auto ledFlashed = false;
    auto ledState = true;

    // Setup UART
    serial.setup(9600);

    /* Infinite loop */
    while (1) {

        // Poll subCycleTime and delay 1 ms
        const auto subCycleTime = OTV0P2BASE::getSubCycleTime();
        if (0U == subCycleTime) { ledFlashed = false; }
        else if((255U == subCycleTime) && (!ledFlashed)) {
            ledFlashed = true;
            ledState = !ledState;
            OTPORT::setPin<gpioPortF, 4>(ledState);
            OTPORT::setPin<gpioPortF, 5>(!ledState);
        }

    }
}
