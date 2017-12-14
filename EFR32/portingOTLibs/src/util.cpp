/*
 * util.c
 *
 *  Created on: 7 Dec 2017
 *      Author: Denzo
 */

#include "util.h"
#include "em_cmu.h"

void setupGPIO() {
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 0);
}
