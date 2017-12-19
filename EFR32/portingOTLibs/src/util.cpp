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
void Serial::setup(const uint32_t baud) {
	    // /* Initialize with default settings and then update fields according to application requirements. */
    USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
    initAsync.baudrate = baud;
    USART_InitAsync(dev, &initAsync);

    // /* Enable I/O and set location */
    dev->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
    dev->ROUTELOC0 = (dev->ROUTELOC0
                        & ~(_USART_ROUTELOC0_TXLOC_MASK
                            | _USART_ROUTELOC0_RXLOC_MASK))
                        | (outputNo << _USART_ROUTELOC0_TXLOC_SHIFT)
                        | (outputNo << _USART_ROUTELOC0_RXLOC_SHIFT);
    /* To avoid false start, configure TX pin as initial high */
    GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_TX_PORT(outputNo), AF_USART0_TX_PIN(outputNo), gpioModePushPull, 1);
    GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_RX_PORT(outputNo), AF_USART0_RX_PIN(outputNo), gpioModeInput, 0);
	isSetup = true;
}

// True if USART not inited.
bool Serial::putchar(const uint8_t c) {
	if (!isSetup) return true;
	USART_Tx(dev, c);
	return false;
}

// returns -1 if USART not inited.
int32_t Serial::print(const uint8_t *const buf, const uint8_t len) {
	if ((nullptr == buf) || (!isSetup)) return -1;
	for (auto *ip = buf; ip != buf+len; ++ip) USART_Tx(dev, *ip);
	return len;
}
int32_t Serial::println(const uint8_t *const buf, const uint8_t len) {
	auto retval = print(buf, len);
	if (-1 == retval) return -1;
	USART_Tx(dev, '\n');
	return (retval + 1);
}

}