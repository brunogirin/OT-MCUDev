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

namespace OTPORT {
void setupClocks();

void setupGPIO();

/**
 * @brief   testing turning a pin on/off with templates
 */
template<GPIO_Port_TypeDef port, unsigned int pin>
void setPin(bool on) {
    if(!on) GPIO_PinOutClear(port, pin);  // LEDs are connected to Vcc
    else GPIO_PinOutSet(port, pin);
}

//bool readPB0() {
//	return GPIO_PinInGet(gpioPortB, PB0_PIN);
//}

}
#endif /* SRC_UTIL_H_ */
