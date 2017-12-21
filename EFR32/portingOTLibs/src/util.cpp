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

    // I2C clock
    CMU_ClockEnable(cmuClock_I2C0, true);

}


void setupGPIO() {
	GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 0);
}

}