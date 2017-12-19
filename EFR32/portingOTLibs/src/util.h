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
void setLED(bool on) {
    if(!on) GPIO_PinOutClear(port, pin);  // LEDs are connected to Vcc
    else GPIO_PinOutSet(port, pin);
}

//bool readPB0() {
//	return GPIO_PinInGet(gpioPortB, PB0_PIN);
//}

class Serial {
private:
    // Flag to prevent UART_Tx locking up when USART not set up first..
    bool isSetup = false;

public:
    void setup(uint32_t baud);
    bool putchar(uint8_t c);
};

}
#endif /* SRC_UTIL_H_ */
