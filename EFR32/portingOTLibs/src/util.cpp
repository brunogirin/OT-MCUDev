/*
 * util.c
 *
 *  Created on: 7 Dec 2017
 *      Author: Denzo
 */

#include "util.h"
#include "em_cmu.h"
#include "em_usart.h"

namespace OTPORT {


void setupClocks() {
	CMU_ClockEnable(cmuClock_GPIO, true);
    // /* USART is a HFPERCLK peripheral. Enable HFPERCLK domain and USART0.
    // * We also need to enable the clock for GPIO to configure pins. */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_USART0, true);
}


void setupGPIO() {
	GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 0);
}

// 
void Serial::setup(uint32_t baud) {
	    // /* Initialize with default settings and then update fields according to application requirements. */
    USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
    initAsync.baudrate = baud;
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
	isSetup = true;
}

//
bool Serial::putchar(uint8_t c) {
	if (!isSetup) return true;
	USART_Tx(USART0, c);
	return false;
}

}