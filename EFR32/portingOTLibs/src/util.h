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

class Serial {
private:
    // Flag to prevent UART_Tx locking up when USART not set up first..
    bool isSetup = false;

    // The usart device we are using.
    // NOTE: This will not work with other ports.
    USART_TypeDef *dev = USART0;

    // What pins to multiplex the USART to.
    static constexpr uint32_t outputNo = 0;

public:
    void setup(uint32_t baud);
    bool putchar(uint8_t c);
    int32_t print(const uint8_t *buf, uint8_t len);
    int32_t println(const uint8_t *buf, uint8_t len);
};

}
#endif /* SRC_UTIL_H_ */
