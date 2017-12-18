/*
 * util.h
 *
 *  Created on: 7 Dec 2017
 *      Author: Denzo
 */

#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

extern "C" {
#include "em_gpio.h"
}

void setupGPIO();

/**
 * @brief   testing turning a pin on/off with templates
 */
template<GPIO_Port_TypeDef port, unsigned int pin>
void setLED(bool on) {
    if(!on) GPIO_PinOutClear(port, pin);  // LEDs are connected to Vcc
    else GPIO_PinOutSet(port, pin);
}

//bool readPB0() {
//	return GPIO_PinInGet(gpioPortB, PB0_PIN);
//}

// /**
//  * @brief	Calculate the number of ticks between interrupts clock should run for.
//  * @param	Time in ms between interrupts
//  * @retval	number of ticks.
//  */
// uint32_t calcSysTickTicks(uint32_t period);



#endif /* SRC_UTIL_H_ */
