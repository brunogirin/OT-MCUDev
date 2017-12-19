extern "C" {
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usart.h"

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



    // // 
    // /* USART is a HFPERCLK peripheral. Enable HFPERCLK domain and USART0.
    // * We also need to enable the clock for GPIO to configure pins. */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_USART0, true);

    // /* Initialize with default settings and then update fields according to application requirements. */
    USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
    initAsync.baudrate = 9600;
    USART_InitAsync(USART0, &initAsync);

    // /* Enable I/O and set location */
    USART0->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
    USART0->ROUTELOC0 = (USART0->ROUTELOC0
                        & ~(_USART_ROUTELOC0_TXLOC_MASK
                            | _USART_ROUTELOC0_RXLOC_MASK))
                        | (0 << _USART_ROUTELOC0_TXLOC_SHIFT)
                        | (0 << _USART_ROUTELOC0_RXLOC_SHIFT);
    /* To avoid false start, configure TX pin as initial high */
    GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_TX_PORT(0), AF_USART0_TX_PIN(0), gpioModePushPull, 1);
    GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_RX_PORT(0), AF_USART0_RX_PIN(0), gpioModeInput, 0);

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

            USART_Tx(USART0, 'a');
        }
    }
}
